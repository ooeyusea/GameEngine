#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "FbxLoader.h"
#include <algorithm>
#include "../Mesh.h"
#include "../MeshPart.h"
#include "../VertexBuffer.h"
#include "../IndexBuffer.h"
#include "../VertexDescription.h"
#include <fstream>
#include "../skeleton_data.h"
#include "../bone_data.h"
#include "../../System.h"
#include "../../util/FuncUnitl.h"
#include "../track.h"
#include "../key_frame.h"
#include "../animation.h"

#undef max
#undef min

#define MAX_BONES 12

namespace loader {
	int cal_add_bone_to_set_cost(std::vector<int>& bone_set,
		const std::vector<std::vector<std::tuple<int, float>>*>& triangle_bone_weights)
	{
		int cost = 0;
		for (const auto& weights : triangle_bone_weights)
		{
			for (const auto& weight : *weights)
			{
				auto bone = std::get<0>(weight);
				auto itr = bone_set.begin();
				while (itr != bone_set.end())
				{
					if (*itr == bone)
					{
						break;
					}
					++itr;
				}

				if (itr == bone_set.end())
				{
					++cost;
				}
			}
		}

		return (cost + bone_set.size() > MAX_BONES) ? -1 : cost;
	}

	void add_bone_to_set(std::vector<int>& bone_set,
		const std::vector<std::vector<std::tuple<int, float>>*>& triangle_bone_weights)
	{
		for (const auto& weights : triangle_bone_weights)
		{
			for (const auto& weight : *weights)
			{
				auto bone = std::get<0>(weight);
				auto itr = bone_set.begin();
				while (itr != bone_set.end())
				{
					if (*itr == bone)
					{
						break;
					}
					++itr;
				}

				if (itr == bone_set.end())
				{
					bone_set.push_back(bone);
				}
			}
		}
	}

	int cal_bone(std::vector<std::vector<int>>& bone_sets,
		const std::vector<std::vector<std::tuple<int, float>>*>& triangle_bone_weights)
	{
		int find = -1;
		int cost = MAX_BONES + 1;
		for (int i = 0; i < bone_sets.size(); ++i)
		{
			int c = cal_add_bone_to_set_cost(bone_sets[i], triangle_bone_weights);
			if (c >= 0 && c < cost)
			{
				find = i;
				cost = c;
			}
		}

		if (find < 0)
		{
			find = bone_sets.size();
			bone_sets.push_back(std::vector<int>());
		}

		add_bone_to_set(bone_sets[find], triangle_bone_weights);

		return find;
	}

	void triangulate_recursive(FbxScene * scene, FbxNode * node)
	{
		FbxNodeAttribute* attribute = node->GetNodeAttribute();

		if (attribute)
		{
			if (attribute->GetAttributeType() == FbxNodeAttribute::eMesh ||
				attribute->GetAttributeType() == FbxNodeAttribute::eNurbs ||
				attribute->GetAttributeType() == FbxNodeAttribute::eNurbsSurface ||
				attribute->GetAttributeType() == FbxNodeAttribute::ePatch)
			{
				FbxGeometryConverter converter(node->GetFbxManager());
				converter.Triangulate(scene, true);
			}
		}

		for (int i = 0; i < node->GetChildCount(); ++i)
		{
			triangulate_recursive(scene, node->GetChild(i));
		}
	}

	void read_position(FbxMesh * mesh, float * data, int& offset, int v)
	{
		auto vertex = mesh->GetControlPointAt(v);
		data[offset++] = (float)vertex[0];
		data[offset++] = (float)vertex[1];
		data[offset++] = (float)vertex[2];
	}

	void read_normal(FbxMesh * mesh, float * data, int& offset, int index, int v)
	{
		auto normals = (FbxLayerElementArray*)&(mesh->GetElementNormal()->GetDirectArray());
		FbxVector4 normal;
		if (mesh->GetElementNormal()->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (mesh->GetElementNormal()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				normals->GetAt<FbxVector4>(mesh->GetElementNormal()->GetIndexArray().GetAt(v), &normal);
			}
			else
			{
				normals->GetAt<FbxVector4>(v, &normal);
			}
		}
		else
		{
			if (mesh->GetElementNormal()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				normals->GetAt<FbxVector4>(mesh->GetElementNormal()->GetIndexArray().GetAt(index), &normal);
			}
			else
			{
				normals->GetAt<FbxVector4>(index, &normal);
			}
		}

		data[offset++] = (float)normal[0];
		data[offset++] = (float)normal[1];
		data[offset++] = (float)normal[2];
	}

	void read_tangent(FbxMesh * mesh, float * data, int& offset, int index, int v)
	{
		auto tangents = (FbxLayerElementArray*)&(mesh->GetElementTangent()->GetDirectArray());
		FbxVector4 tangent;
		if (mesh->GetElementTangent()->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (mesh->GetElementTangent()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				tangents->GetAt<FbxVector4>(mesh->GetElementTangent()->GetIndexArray().GetAt(v), &tangent);
			}
			else
			{
				tangents->GetAt<FbxVector4>(v, &tangent);
			}
		}
		else
		{
			if (mesh->GetElementTangent()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				tangents->GetAt<FbxVector4>(mesh->GetElementTangent()->GetIndexArray().GetAt(index), &tangent);
			}
			else
			{
				tangents->GetAt<FbxVector4>(index, &tangent);
			}
		}

		data[offset++] = (float)tangent[0];
		data[offset++] = (float)tangent[1];
		data[offset++] = (float)tangent[2];
	}

	void read_binormal(FbxMesh * mesh, float * data, int& offset, int index, int v)
	{
		auto binormals = (FbxLayerElementArray*)&(mesh->GetElementBinormal()->GetDirectArray());
		FbxVector4 binormal;
		if (mesh->GetElementBinormal()->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (mesh->GetElementBinormal()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				binormals->GetAt<FbxVector4>(mesh->GetElementBinormal()->GetIndexArray().GetAt(v), &binormal);
			}
			else
			{
				binormals->GetAt<FbxVector4>(v, &binormal);
			}
		}
		else
		{
			if (mesh->GetElementBinormal()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				binormals->GetAt<FbxVector4>(mesh->GetElementBinormal()->GetIndexArray().GetAt(index), &binormal);
			}
			else
			{
				binormals->GetAt<FbxVector4>(index, &binormal);
			}
		}

		data[offset++] = (float)binormal[0];
		data[offset++] = (float)binormal[1];
		data[offset++] = (float)binormal[2];
	}

	void read_color(FbxMesh * mesh, float * data, int& offset, int index, int v)
	{
		auto colors = (FbxLayerElementArray*)&(mesh->GetElementVertexColor()->GetDirectArray());
		FbxColor color;
		if (mesh->GetElementVertexColor()->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (mesh->GetElementVertexColor()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				colors->GetAt<FbxColor>(mesh->GetElementVertexColor()->GetIndexArray().GetAt(v), &color);
			}
			else
			{
				colors->GetAt<FbxColor>(v, &color);
			}
		}
		else
		{
			if (mesh->GetElementVertexColor()->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				colors->GetAt<FbxColor>(mesh->GetElementVertexColor()->GetIndexArray().GetAt(index), &color);
			}
			else
			{
				colors->GetAt<FbxColor>(index, &color);
			}
		}

		data[offset++] = (float)color.mRed;
		data[offset++] = (float)color.mGreen;
		data[offset++] = (float)color.mBlue;
		data[offset++] = (float)color.mAlpha;
	}

	inline void read_uv(FbxMesh * mesh, float * data, int& offset, int uv_index, int index, int v, bool filp)
	{

		auto uvs = ((FbxLayerElementArray*)&(mesh->GetElementUV(uv_index)->GetDirectArray()));
		FbxVector2 uv;
		if (mesh->GetElementUV(uv_index)->GetMappingMode() == FbxGeometryElement::eByControlPoint)
		{
			if (mesh->GetElementUV(uv_index)->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				uvs->GetAt<FbxVector2>(mesh->GetElementUV(uv_index)->GetIndexArray().GetAt(v), &uv);
			}
			else
			{
				uvs->GetAt<FbxVector2>(v, &uv);
			}
		}
		else
		{
			if (mesh->GetElementUV(uv_index)->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
			{
				uvs->GetAt<FbxVector2>(mesh->GetElementUV(uv_index)->GetIndexArray().GetAt(index), &uv);
			}
			else
			{
				uvs->GetAt<FbxVector2>(index, &uv);
			}
		}

		if (filp)
		{
			data[offset++] = (float)uv.mData[0];
			data[offset++] = 1.0f - (float)uv.mData[1];
		}
		else
		{
			data[offset++] = (float)uv.mData[0];
			data[offset++] = (float)uv.mData[1];
		}
	}

	int cal_bone_index(std::vector<int>& bone_set, int bone)
	{
		for (int i = 0; i < bone_set.size(); ++i)
		{
			if (bone_set[i] == bone)
				return i;
		}
		return -1;
	}

	void read_blend_weight(MeshInfo& info, float * data, int &offset, int weight_index, int i, int index, int v)
	{
		auto weights = info.point_blend_weights[v];
		auto s = weights.size();

		auto bone_set = info.part_bones[info.triangle_part[i]][info.triangle_part_bones_map[i]];
		data[offset++] = weight_index < s ? (float)cal_bone_index(bone_set, std::get<0>(weights[weight_index])) : 0.f;
		data[offset++] = weight_index < s ? std::get<1>(weights[weight_index]) : 0.0f;
	}

	FbxLoader::FbxLoader()
		: _vertex_format(0)
		, _vertex_size(0)
		, _skeleton(nullptr)
	{
	}


	FbxLoader::~FbxLoader()
	{
		if (_skeleton != nullptr)
			delete _skeleton;
		_skeleton = nullptr;
	}

	bool FbxLoader::load_file(const std::string& file)
	{
		_file = file;

		FbxManager* manager = FbxManager::Create();

		FbxIOSettings * ios_setting = FbxIOSettings::Create(manager, IOSROOT);
		manager->SetIOSettings(ios_setting);

		FbxImporter* importer = FbxImporter::Create(manager, "");

		if (!importer->Initialize(file.c_str(), -1, manager->GetIOSettings()))
		{
			return false;
		}

		FbxScene* scene = FbxScene::Create(manager, "");
		importer->Import(scene);
		importer->Destroy();

		FbxAxisSystem::OpenGL.ConvertScene(scene);

		triangulate_recursive(scene, scene->GetRootNode());

		setup(scene->GetRootNode());
		cal_vertex_size();

		travel(scene->GetRootNode());
		construct_skeleton(scene->GetRootNode());
		read_animation(scene);

		manager->Destroy();

		return true;
	}

	lly::Mesh * FbxLoader::convert_to_resource()
	{
		lly::Mesh * ret = new lly::Mesh();
		lly::VertexBuffer * vb = new lly::VertexBuffer;
		vb->load_data((const char*)_vertices.data(), _vertices.size() * sizeof(float), lly::VertexUsage::STATIC_DRAW);
		ret->set_vertex_buffer(vb);

		int i = 0;
		bool has_skeleton = false;
		for (auto& part : _parts)
		{
			++i;
			//if (i != 3)
			//	continue;
			lly::VertexDescription desc(lly::DrawType::TRIANGLES, part.indices.size());
			descript_vertex(desc);

			auto mesh_part = ret->create_part();

			lly::IndexBuffer * ib = new lly::IndexBuffer;
			ib->load_indices((const char*)part.indices.data(), part.indices.size() * sizeof(int));
			ib->gen(vb, desc);

			mesh_part->set_index_buffer(ib);

			if (!part.bones.empty())
			{
				for (auto& bone : part.bones)
					mesh_part->add_bone(std::get<0>(bone), std::get<1>(bone));
				has_skeleton = true;
			}
		}

		if (has_skeleton)
		{
			auto skeleton = lly::System::instance().get_resource_manager().load_skeleton(_file, _skeleton);
			ret->set_skeleton(skeleton);
		}
		else
			delete _skeleton;
		_skeleton = nullptr;
		return ret;
	}

	void FbxLoader::setup(FbxNode * root)
	{
		if (root == nullptr)
			return;

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				setup_node(node);
			}
		}

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				setup(node);
			}
		}
	}

	void FbxLoader::setup_node(FbxNode * node)
	{
		if (node == nullptr)
			return;

		FbxMesh * mesh = node->GetMesh();
		if (mesh == nullptr)
			return;

		MeshInfo& info = _mesh_infos[mesh];

		_vertex_format |= Attribute::POSITION;
		if (mesh->GetElementNormalCount() > 0)
			_vertex_format |= Attribute::NORMAL;
		if (mesh->GetElementVertexColorCount() > 0)
			_vertex_format |= Attribute::COLOR;
		if (mesh->GetElementTangentCount() > 0)
			_vertex_format |= Attribute::TANGENT;

		for (int i = 0; i < 8; ++i)
		{
			if (i < mesh->GetElementUVCount())
				_vertex_format |= (Attribute::TEXCOOD0 << i);
		}

		if (mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
		{
			for (unsigned int i = 0; i < 4; i++)
				_vertex_format |= (Attribute::WEIGHT0 << i);

			FbxSkin * skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));
			int cluster_count = skin->GetClusterCount();
			for (int i = 0; i < cluster_count; ++i)
			{
				FbxCluster *cluster = skin->GetCluster(i);
				int index_count = cluster->GetControlPointIndicesCount();
				int * cluster_indices = cluster->GetControlPointIndices();
				double * cluster_weights = cluster->GetControlPointWeights();

				for (int j = 0; j < index_count; ++j) {
					if (cluster_indices[j] < 0 || cluster_indices[j] >= mesh->GetControlPointsCount() || cluster_weights[j] == 0.0)
						continue;
					info.point_blend_weights[cluster_indices[j]].push_back(std::make_tuple(i, (float)cluster_weights[j]));
				}
			}

			for (auto itr : info.point_blend_weights)
			{
				std::sort(itr.second.begin(), itr.second.end(), [](const std::tuple<int, float>& a, const std::tuple<int, float>& b){
					return std::get<1>(a) > std::get<1>(b);
				});

				if (itr.second.size() > 4)
				{
					itr.second.resize(4);
				}

				float len = 0.0f;
				for (auto& weight : itr.second)
					len += std::get<1>(weight);

				if (len > 0.0f)
				{
					for (auto& weight : itr.second)
						std::get<1>(weight) = std::get<1>(weight) / len;
				}
			}
		}

		int triangle_count = mesh->GetPolygonCount();
		for (int i = 0; i < triangle_count; ++i)
		{
			int element_material_count = mesh->GetElementMaterialCount();
			for (int j = 0; j < element_material_count; ++j)
			{
				auto index = mesh->GetElementMaterial(j)->GetIndexArray()[i];
				index = (index < 0 ? -1 : index);
				info.triangle_part[i] = index;

				if (mesh->GetDeformerCount(FbxDeformer::eSkin) > 0)
				{
					std::vector<std::vector<std::tuple<int, float>>*> triangle_bone_weights;
					for (int k = 0; k < 3; ++k)
						triangle_bone_weights.push_back(&info.point_blend_weights[mesh->GetPolygonVertex(i, k)]);

					info.triangle_part_bones_map[i] = cal_bone(info.part_bones[index], triangle_bone_weights);
				}
			}
		}

		for (auto& itr : info.part_bones)
		{
			for (auto& bone_set : itr.second)
			{
				std::sort(bone_set.begin(), bone_set.end());
			}
		}
	}

	void FbxLoader::travel(FbxNode * root)
	{
		if (root == nullptr)
			return;

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				read_mesh(node);
			}
		}

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				travel(node);
			}
		}
	}

	FbxAMatrix GetGeometry(FbxNode* pNode)
	{
		const FbxVector4 lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		const FbxVector4 lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		const FbxVector4 lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		return FbxAMatrix(lT, lR, lS);
	}

	void FbxLoader::read_mesh(FbxNode * node)
	{
		FbxMesh * mesh = node->GetMesh();
		if (mesh == nullptr)
			return;

		MeshInfo& info = _mesh_infos[mesh];
		FbxSkin * skin = static_cast<FbxSkin*>(mesh->GetDeformer(0, FbxDeformer::eSkin));

		std::vector<std::vector<MeshPart>> parts(node->GetMaterialCount());
		for (int i = 0; i < node->GetMaterialCount(); ++i)
		{
			int n = info.part_bones[i].size();
			int m = (n == 0 ? 1 : n);
			parts[i].resize(m);
			for (int j = 0; j < m; ++j)
			{
				FbxAMatrix global = node->EvaluateGlobalTransform();
				FbxAMatrix geometry = GetGeometry(node);
				FbxAMatrix p = (global * geometry).Inverse();
				parts[i][j].pre_matrix = glm::mat4(
					p.mData[0][0], p.mData[0][1], p.mData[0][2], p.mData[0][3],
					p.mData[1][0], p.mData[1][1], p.mData[1][2], p.mData[1][3],
					p.mData[2][0], p.mData[2][1], p.mData[2][2], p.mData[2][3],
					p.mData[3][0], p.mData[3][1], p.mData[3][2], p.mData[3][3]
				);

				if (j < n)
				{
					for (auto& bone : info.part_bones[i][j])
					{
						auto fbx_bone = skin->GetCluster(bone);
						std::string name = fbx_bone->GetLink()->GetName();

						FbxAMatrix reference;
						fbx_bone->GetTransformMatrix(reference);
						reference *= geometry;
						FbxAMatrix init;
						fbx_bone->GetTransformLinkMatrix(init);
						FbxAMatrix m = init.Inverse() * reference;

						glm::vec3 position(m.GetT().mData[0], m.GetT().mData[1], m.GetT().mData[2]);
						glm::quat rotation(glm::quat(m.GetQ().mData[3], m.GetQ().mData[0], m.GetQ().mData[1], m.GetQ().mData[2]));
						glm::vec3 scale(m.GetS().mData[0], m.GetS().mData[1], m.GetS().mData[2]);

						glm::mat4 inverse_bindpose(
							m.mData[0][0], m.mData[0][1], m.mData[0][2], m.mData[0][3],
							m.mData[1][0], m.mData[1][1], m.mData[1][2], m.mData[1][3],
							m.mData[2][0], m.mData[2][1], m.mData[2][2], m.mData[2][3],
							m.mData[3][0], m.mData[3][1], m.mData[3][2], m.mData[3][3]
						);

						glm::mat4 transform = lly_util::to_sqt(position, rotation, scale);
						FbxAMatrix tmp;
						tmp.SetTQS(m.GetT(), m.GetQ(), m.GetS());
						glm::mat4 test = glm::mat4_cast(rotation);

						bool acc = m.IsRightHand();

						parts[i][j].bones.push_back(std::make_tuple(name, inverse_bindpose));
					}
				}
			}
		}

		float *vertex = new float[_vertex_size];
		int index = 0;
		for (int i = 0; i < mesh->GetPolygonCount(); ++i) 
		{
			MeshPart& part = parts[info.triangle_part[i]][info.triangle_part_bones_map[i]];

			for (int j = 0; j < 3; ++j) 
			{
				auto v = mesh->GetPolygonVertex(i, j);
				read_vertex(node, vertex, i, index, v);

				part.indices.push_back(add_vertex(part, vertex));
				++index;
			}
		}
		delete[] vertex;

		for (auto& sub_parts : parts)
		{
			for (auto& part : sub_parts)
			{
				if (part.indices.empty())
					continue;

				_parts.push_back(part);
			}
		}
	}

	void FbxLoader::cal_vertex_size()
	{
		_vertex_size = 0;
		if ((_vertex_format & POSITION) > 0)
			_vertex_size += 3;
		if ((_vertex_format & NORMAL) > 0)
			_vertex_size += 3;
		if ((_vertex_format & COLOR) > 0)
			_vertex_size += 4;
		if ((_vertex_format & TANGENT) > 0)
			_vertex_size += 3;

		for (int i = 0; i < 8; ++i)
		{
			if ((_vertex_format & (TEXCOOD0 << i)) > 0)
				_vertex_size += 2;
		}

		for (int i = 0; i < 8; ++i)
		{
			if ((_vertex_format & (WEIGHT0 << i)) > 0)
				_vertex_size += 2;
		}
	}

	void FbxLoader::read_vertex(FbxNode * node, float * vertex, int poly_index, int index, int v)
	{
		int offset = 0;
		FbxMesh * mesh = node->GetMesh();
		MeshInfo& info = _mesh_infos[mesh];

		if ((_vertex_format & POSITION) > 0)
			read_position(mesh, vertex, offset, v);
		if ((_vertex_format & NORMAL) > 0)
			read_normal(mesh, vertex, offset, index, v);
		if ((_vertex_format & COLOR) > 0)
			read_color(mesh, vertex, offset, index, v);
		if ((_vertex_format & TANGENT) > 0)
			read_tangent(mesh, vertex, offset, index, v);

		for (int i = 0; i < 8; ++i)
		{
			if ((_vertex_format & (TEXCOOD0 << i)) > 0)
				read_uv(mesh, vertex, offset, i, index, v, true);
		}

		for (int i = 0; i < 8; ++i)
		{
			if ((_vertex_format & (WEIGHT0 << i)) > 0)
				read_blend_weight(info, vertex, offset, i, poly_index, index, v);
		}
	}

	int FbxLoader::add_vertex(MeshPart& part, float * vertex)
	{
		for (int i = 0; i < _vertex_count; ++i)
		{
			bool same = true;
			for (int j = 0; j < _vertex_size; ++j)
			{
				if (_vertices[i * _vertex_size + j] != vertex[j])
				{
					same = false;
					break;
				}
			}
			if (same)
				return i;
		}

		for (int j = 0; j < _vertex_size; ++j)
		{
			_vertices.push_back(vertex[j]);
		}

		return _vertex_count++;
	}

	void FbxLoader::descript_vertex(lly::VertexDescription& desc)
	{
		int offset = 0;
		if ((_vertex_format & POSITION) > 0)
		{
			desc.add(lly::VertexAttribPosition::POSITION, 3, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 3;
		}

		if ((_vertex_format & NORMAL) > 0)
		{
			desc.add(lly::VertexAttribPosition::NORMAL, 3, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 3;
		}

		if ((_vertex_format & COLOR) > 0)
		{
			desc.add(lly::VertexAttribPosition::COLOR, 4, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 4;
		}

		if ((_vertex_format & TANGENT) > 0)
		{
			desc.add(lly::VertexAttribPosition::TANGENT, 3, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 3;
		}

		if ((_vertex_format & TEXCOOD0) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD0, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD1) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD1, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD2) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD2, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD3) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD3, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD4) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD4, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD5) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD5, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD6) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD6, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & TEXCOOD7) > 0)
		{
			desc.add(lly::VertexAttribPosition::TEXCOOD7, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & WEIGHT0) > 0)
		{
			desc.add(lly::VertexAttribPosition::WEIGHT0, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & WEIGHT1) > 0)
		{
			desc.add(lly::VertexAttribPosition::WEIGHT1, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & WEIGHT2) > 0)
		{
			desc.add(lly::VertexAttribPosition::WEIGHT2, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}

		if ((_vertex_format & WEIGHT3) > 0)
		{
			desc.add(lly::VertexAttribPosition::WEIGHT3, 2, lly::DataType::FLOAT, false, _vertex_size * sizeof(float), offset * sizeof(float));
			offset += 2;
		}
	}

	void FbxLoader::construct_bone(FbxNode * root)
	{
		auto parent = _skeleton->create_bone(root->GetName());
		parent->set_name(root->GetName());

		FbxAMatrix &m = root->EvaluateLocalTransform();
		parent->set_position(m.GetT().mData[0], m.GetT().mData[1], m.GetT().mData[2]);
		parent->set_rotation(glm::quat(m.GetQ().mData[3], m.GetQ().mData[0], m.GetQ().mData[1], m.GetQ().mData[2]));
		parent->set_scale(m.GetS().mData[0], m.GetS().mData[1], m.GetS().mData[2]);

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				construct_bone(node);

				auto child = _skeleton->get_bone(node->GetName());
				parent->add_child(child);
				child->set_parent(parent);
			}
		}
	}

	void FbxLoader::construct_skeleton(FbxNode * root)
	{
		_skeleton = new lly::SkeletonData();
		auto parent = _skeleton->create_bone("root");
		parent->set_name("root");
		_skeleton->set_root_bone(parent);

		for (int i = 0; i < root->GetChildCount(); i++)
		{
			FbxNode * node = root->GetChild(i);
			if (node != nullptr)
			{
				construct_bone(node);

				auto child = _skeleton->get_bone(node->GetName());
				parent->add_child(child);
				child->set_parent(parent);
			}
		}
	}

	void update_anim_time(FbxAnimCurve * curve, AnimInfo &ts, float &anim_start, const float anim_stop)
	{
		FbxTimeSpan fts;
		curve->GetTimeInterval(fts);
		const FbxTime start = fts.GetStart();
		const FbxTime stop = fts.GetStop();

		ts.start = std::max(anim_start, std::min(ts.start, (float)(start.GetMilliSeconds())));
		ts.stop = std::min(anim_stop, std::max(ts.stop, (float)stop.GetMilliSeconds()));
		ts.framerate = std::max(ts.framerate, (float)stop.GetFrameRate(FbxTime::eDefaultMode));
	}

	void FbxLoader::read_animation(FbxScene * scene)
	{
		int animation_count = scene->GetSrcObjectCount<FbxAnimStack>();
		for (int i = 0; i < animation_count; ++i)
		{
			std::unordered_map<FbxNode *, AnimInfo> affect_nodes;

			auto anim_stack = scene->GetSrcObject<FbxAnimStack>(i);

			FbxTimeSpan anim_time_span = anim_stack->GetLocalTimeSpan();
			float start = (float)(anim_time_span.GetStart().GetMilliSeconds());
			float stop = (float)(anim_time_span.GetStop().GetMilliSeconds());
			if (stop <= start)
				stop = 999999999.0f;

			int layer_count = anim_stack->GetMemberCount<FbxAnimLayer>();
			for (int j = 0; j < layer_count; ++j) 
			{
				auto layer = anim_stack->GetMember<FbxAnimLayer>(j);

				int curve_node_count = layer->GetSrcObjectCount<FbxAnimCurveNode>();
				for (int n = 0; n < curve_node_count; ++n) 
				{
					auto curve_node = layer->GetSrcObject<FbxAnimCurveNode>(n);

					int property_count = curve_node->GetDstPropertyCount();
					for (int o = 0; o < property_count; ++o) 
					{
						FbxProperty prop = curve_node->GetDstProperty(o);
						FbxNode *node = static_cast<FbxNode *>(prop.GetFbxObject());
						if (node) 
						{
							FbxString prop_name = prop.GetName();
							if (prop_name == "DeformPercent")
								continue;

							// Only add translation, scaling or rotation
							if ((!node->LclTranslation.IsValid() || prop_name != node->LclTranslation.GetName()) &&
								(!node->LclScaling.IsValid() || prop_name != node->LclScaling.GetName()) &&
								(!node->LclRotation.IsValid() || prop_name != node->LclRotation.GetName()))
								continue;

							FbxAnimCurve * curve = nullptr;
							AnimInfo ts;
							ts.translate = (prop_name == node->LclTranslation.GetName());
							ts.rotate = (prop_name == node->LclRotation.GetName());
							ts.scale = (prop_name == node->LclScaling.GetName());

							if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_X))
								update_anim_time(curve, ts, start, stop);
							if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Y))
								update_anim_time(curve, ts, start, stop);
							if (curve = prop.GetCurve(layer, FBXSDK_CURVENODE_COMPONENT_Z))
								update_anim_time(curve, ts, start, stop);

							affect_nodes[node] += ts;
						}
					}
				}
			}

			if (affect_nodes.empty())
				return;

			auto animation = _skeleton->create_animation(anim_stack->GetName());
			animation->set_length(((float)(stop - start)) / 1000.0f);

			scene->SetCurrentAnimationStack(anim_stack);
			for (auto itr = affect_nodes.begin(); itr != affect_nodes.end(); ++itr) 
			{
				auto node = _skeleton->get_bone(itr->first->GetName());
				if (!node)
					continue;

				lly::NodeTrack * track = new lly::NodeTrack;
				track->set_target(itr->first->GetName());
				track->set_total_time(((float)itr->second.stop - itr->second.start) / 1000.0f);

				float step_size = itr->second.framerate <= 0.f ? itr->second.stop - itr->second.start : 1000.f / itr->second.framerate;
				float last = itr->second.stop + step_size * 0.5f;

				for (float time = itr->second.start; time <= last; time += step_size)
				{
					time = std::min(time, itr->second.stop);

					FbxTime fbx_time;
					fbx_time.SetMilliSeconds((FbxLongLong)time);

					lly::TransformFrame * frame = new lly::TransformFrame(time - start);

					auto m = itr->first->EvaluateLocalTransform(fbx_time);
					frame->set_position(glm::vec3(m.GetT().mData[0], m.GetT().mData[1], m.GetT().mData[2]));
					frame->set_rotation(glm::quat(m.GetQ().mData[3], m.GetQ().mData[0], m.GetQ().mData[1], m.GetQ().mData[2]));
					frame->set_scale(glm::vec3(m.GetS().mData[0], m.GetS().mData[1], m.GetS().mData[2]));

					track->add_key_frame(frame);
				}

				animation->add_track(track);
			}
		}
	}
}
