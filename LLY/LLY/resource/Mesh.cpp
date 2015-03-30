#include <glm/gtc/matrix_transform.hpp>
#include "Mesh.h"
#include "MeshPart.h"
#include "../System.h"
#include "material/Material.h"
#include "../util/FileUtil.h"
#include "../util/Macros.h"
#include <regex>
#include "VertexBuffer.h"
#include "VertexDescription.h"
#include <fbxsdk.h>
#include <fstream>
#include <map>

namespace lly {
	Mesh::Mesh()
		: _vb(nullptr)
		, _skeleton(0)
	{
	}

	Mesh::~Mesh()
	{
		if (_vb != nullptr)
		{
			delete _vb;
			_vb = nullptr;
		}

		for (auto& unit : _parts)
		{
			delete unit;
		}

		_parts.clear();
	}

	MeshPart * Mesh::create_part()
	{
		_parts.push_back(new MeshPart);
		return *_parts.rbegin();
	}

	void Mesh::add_part(MeshPart * part)
	{
		_parts.push_back(part);
	}

	MeshPart * Mesh::get_part(unsigned int index) const
	{
		if (index < _parts.size())
		{
			return _parts[index];
		}

		return nullptr;
	}

	void Mesh::draw()
	{
		int i = 0;
		for (auto& part : _parts)
		{
			auto material = System::instance().get_resource_manager().get_material(part->get_material());
			material->draw(*part);
			++i;
		}
	}

	void Mesh::draw(Skeleton * skeleton)
	{
		int i = 0;
		for (auto& part : _parts)
		{
			auto material = System::instance().get_resource_manager().get_material(part->get_material());
			System::instance().set_matrix_palette(part->get_matrix_palette(skeleton));

			//if (i == 2)
			material->draw(*part);
			++i;
		}
	}

	namespace mesh_loader {

		template <typename T>
		class AutoDeletor
		{
		public:
			explicit AutoDeletor(T* t) : _t(t) {}
			~AutoDeletor()
			{
				LLY_SAFE_DELETE(_t);
			}

			void no() { _t = nullptr; }

		private:
			T* _t;
		};

		std::string get_word(std::list<std::string>& words)
		{
			if (words.empty())
				throw std::logic_error("words eof");
			std::string ret = *words.begin();
			words.pop_front();
			return std::move(ret);
		}

		int to_int(const std::string& word)
		{
			std::regex pattern("^[+-]?[0-9]+$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse int error");
			}

			return atoi(word.c_str());
		}

		float to_float(const std::string& word)
		{
			std::regex pattern("^[+-]?([0-9]+\\.[0-9]+)([eE][+-]?[0-9]+)?$");
			if (!std::regex_match(word, pattern))
			{
				throw std::logic_error("parse float error");
			}

			return (float)atof(word.c_str());
		}

		bool to_bool(const std::string& word)
		{
			if (word == "true")
				return true;
			else if (word == "false")
				return false;
			throw std::logic_error("parse switch error");
		}

		DrawType to_draw_type(const std::string& word)
		{
			if (word == "points")
				return DrawType::POINTS;
			else if (word == "line_strip")
				return DrawType::LINE_STRIP;
			else if (word == "line_loop")
				return DrawType::LINE_LOOP;
			else if (word == "lines")
				return DrawType::LINES;
			else if (word == "triangle_strip")
				return DrawType::TRIANGLE_STRIP;
			else if (word == "triangle_fan")
				return DrawType::TRIANGLE_FAN;
			else if (word == "triangles")
				return DrawType::TRIANGLES;
			else if (word == "quad_strip")
				return DrawType::QUAD_STRIP;
			else if (word == "quads")
				return DrawType::QUADS;
			else if (word == "polygon")
				return DrawType::POLYGON;

			throw std::logic_error("parse draw type error");
		}

		VertexAttribPosition to_vertex_attrib_position(const std::string& word)
		{
			if (word == "position")
				return VertexAttribPosition::POSITION;
			else if (word == "normal")
				return VertexAttribPosition::NORMAL;
			else if (word == "color")
				return VertexAttribPosition::COLOR;
			else if (word == "tangent")
				return VertexAttribPosition::TANGENT;
			else if (word == "texcood_0")
				return VertexAttribPosition::TEXCOOD0;
			else if (word == "texcood_1")
				return VertexAttribPosition::TEXCOOD1;
			else if (word == "texcood_2")
				return VertexAttribPosition::TEXCOOD2;
			else if (word == "texcood_3")
				return VertexAttribPosition::TEXCOOD3;
			else if (word == "texcood_4")
				return VertexAttribPosition::TEXCOOD4;
			else if (word == "texcood_5")
				return VertexAttribPosition::TEXCOOD5;
			else if (word == "texcood_6")
				return VertexAttribPosition::TEXCOOD6;
			else if (word == "texcood_7")
				return VertexAttribPosition::TEXCOOD7;
			else if (word == "weight_0")
				return VertexAttribPosition::WEIGHT0;
			else if (word == "weight_1")
				return VertexAttribPosition::WEIGHT1;
			else if (word == "weight_2")
				return VertexAttribPosition::WEIGHT2;
			else if (word == "weight_3")
				return VertexAttribPosition::WEIGHT3;

			throw std::logic_error("parse draw type error");
		}

		DataType to_data_type(const std::string& word)
		{
			if (word == "byte")
				return DataType::BYTE;
			else if (word == "ubyte")
				return DataType::UNSIGNED_BYTE;
			else if (word == "short")
				return DataType::SHORT;
			else if (word == "ushort")
				return DataType::UNSIGNED_SHORT;
			else if (word == "fixed")
				return DataType::FIXED;
			else if (word == "float")
				return DataType::FLOAT;

			throw std::logic_error("parse draw type error");
		}

		VertexDescription load_desc(std::list<std::string>& words)
		{
			VertexDescription ret(to_draw_type(get_word(words)), to_int(get_word(words)));

			std::string tmp = get_word(words);
			if (tmp != "{")
			{
				throw std::logic_error("load vertex buffer but miss {");
			}

			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "size")
				{
					ret.add(to_vertex_attrib_position(get_word(words)), to_int(get_word(words)), to_data_type(get_word(words)), to_bool(get_word(words)), to_int(get_word(words)), to_int(get_word(words)));
				}
				else if (tmp == "}")
				{
					return std::move(ret);
				}
				else
				{
					throw std::logic_error("load vertex buffer unknown data");
				}
			}

			throw std::logic_error("load vertex buffer but miss }");
		}

		VertexBuffer* load_vertex(std::list<std::string>& words)
		{
			std::string tmp = get_word(words);
			if (tmp != "{")
			{
				throw std::logic_error("load vertex buffer but miss {");
			}
			tmp = get_word(words);
			if (tmp != "desc")
			{
				throw std::logic_error("load vertex buffer but miss desc");
			}
			VertexDescription desc = load_desc(words);

			VertexBuffer * ret = new VertexBuffer;
			AutoDeletor<VertexBuffer> deletor(ret);
			std::vector<float> data;
			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "}")
				{
					deletor.no();
					return ret;
				}
				else
				{
					data.push_back(to_float(tmp));
				}
			}

			throw std::logic_error("load vertex buffer but miss }");
		}

		MeshPart* load_part(std::list<std::string>& words)
		{
			std::string tmp = get_word(words);
			if (tmp != "{")
			{
				throw std::logic_error("load one mesh part but miss {");
			}

			MeshPart * ret = new MeshPart;
			AutoDeletor<MeshPart> deletor(ret);
			while (!words.empty())
			{
				tmp = get_word(words);
				if (tmp == "vertex")
				{
					VertexBuffer * vertex_buffer = load_vertex(words);
					//ret->set_vertex_buffer(vertex_buffer);
				}
				else if (tmp == "material")
				{
					std::string path = get_word(words);
					auto material = System::instance().get_resource_manager().load_material(path);
					ret->set_material(material);
				}
				else if (tmp == "}")
				{
					deletor.no();
					return ret;
				}
				else
				{
					throw std::logic_error("load one mesh part unknown data");
				}
			}

			throw std::logic_error("load one mesh part but miss }");
		}

		Mesh* load(std::list<std::string>& words)
		{
			Mesh * ret = new Mesh;
			while (!words.empty())
			{
				MeshPart * part = load_part(words);
				ret->add_part(part);
			}
			return ret;
		}
		
		Mesh * load_mesh(const std::string& filename)
		{
			lly_util::Data data = lly_util::load_from_file(filename);
			std::list<std::string> words;
			int offset = 0;
			while (true)
			{
				if (!data.skip_white_space(offset))
					break;

				std::string word;

				if (data.cmp("/*", offset))
				{
					if (!data.next("*/", offset))
						throw std::logic_error("material format error");
				}
				else if (data.cmp("//", offset))
				{
					if (!data.next("\n", offset))
						throw std::logic_error("material format error");
				}
				else if (data.get_word(offset, word))
				{
					words.push_back(word);
				}
				else
				{
					throw std::logic_error("material format error");
				}
			}

			return load(words);
		}

		/*glm::mat4 load_matrix(FbxScene * scene, FbxNode * node)
		{
			glm::mat4 ret;
			glm::mat4 test(1.0f);

			FbxAMatrix& worldMatrix = node->EvaluateGlobalTransform();
			for (int i = 0; i<4; i++)
			{
				for (int j = 0; j<4; j++)
				{
					ret[i][j] = static_cast<float>(worldMatrix.Get(i, j));
				}
			}

			glm::vec3 translate(worldMatrix.GetT().mData[0], worldMatrix.GetT().mData[1], worldMatrix.GetT().mData[2]);
			glm::vec3 rotate(worldMatrix.GetR().mData[0], worldMatrix.GetR().mData[1], worldMatrix.GetR().mData[2]);
			glm::vec3 scale(worldMatrix.GetS().mData[0], worldMatrix.GetS().mData[1], worldMatrix.GetS().mData[2]);

			test = glm::rotate(test, (float)(rotate.x * M_PI / 180.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			test = glm::rotate(test, (float)(rotate.y * M_PI / 180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			test = glm::rotate(test, (float)(rotate.z * M_PI / 180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			test = glm::scale(test, scale);
			test = glm::translate(test, translate);

			return ret;
		}

		void load_fbx_node(FbxScene * scene, FbxNode * node, Mesh * ret)
		{
			FbxMesh * mesh = node->GetMesh();
			if (mesh == nullptr)
				return;

			if (mesh->GetControlPointsCount() <= 0)
				return;

			std::map<int, std::vector<std::tuple<int, float>>> point_blend_weights;
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
					point_blend_weights[cluster_indices[j]].push_back(std::make_tuple(i, (float)cluster_weights[j]));
				}
			}

			std::ofstream file("test.txt", std::ios_base::app);
			file << node->GetName() << std::endl;

			VertexBuffer * buffer = new VertexBuffer;

			int triangle_count = mesh->GetPolygonCount();
			float * vertices = new float[triangle_count * 3 * 11];
			int index = 0;
			//int * indics = new int[triangle_count * 3];
			for (int i = 0; i < triangle_count; ++i)
			{
				for (int j = 0; j < 3; ++j)
				{
					//indics[i * 3 + j] = mesh->GetPolygonVertex(i, j);
					auto pos = mesh->GetPolygonVertex(i, j);
					auto vertex = mesh->GetControlPointAt(pos);

					vertices[index++] = vertex.mData[0];
					vertices[index++] = vertex.mData[1];
					vertices[index++] = vertex.mData[2];

					file << vertex.mData[0] << "," << vertex.mData[1] << "," << vertex.mData[2] << std::endl;
				
					int k = 0;
					for (auto weight : point_blend_weights[i])
					{
						if (k >= 4)
							break;

						vertices[index++] = (float)std::get<0>(weight);
						vertices[index++] = std::get<1>(weight);

						file << "," << std::get<0>(weight) << "," << std::get<1>(weight);

						++k;
					}
					
					if (k < 4)
						index += (4 - k) * 2;
				}
			}


			buffer->load_data((const char *)vertices, sizeof(float)* triangle_count * 3  * 11, VertexUsage::STATIC_DRAW);
			delete[] vertices;

			VertexDescription description(DrawType::TRIANGLES, triangle_count * 3);
			description.add(VertexAttribPosition::POSITION, 3, DataType::FLOAT, false, 11 * sizeof(float), 0);
			//description.add(VertexAttribPosition::TEXCOOD0, 2, DataType::FLOAT, false, 0);
			//description.add(VertexAttribPosition::WEIGHT0, 2, DataType::FLOAT, false, 0);
			//description.add(VertexAttribPosition::WEIGHT1, 2, DataType::FLOAT, false, 0);
			//description.add(VertexAttribPosition::WEIGHT2, 2, DataType::FLOAT, false, 0);
			//description.add(VertexAttribPosition::WEIGHT3, 2, DataType::FLOAT, false, 0);
			buffer->gen(description);

			auto part = ret->create_part();
			part->set_vertex_buffer(buffer);
			part->set_matrix(load_matrix(scene, node));
		}

		void search_and_load_fbx(FbxScene * scene, FbxNode * root, Mesh * ret)
		{
			if (root == nullptr)
				return;

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				FbxNode * node = root->GetChild(i);
				if (node != nullptr)
				{
					load_fbx_node(scene, node, ret);
				}
			}

			for (int i = 0; i < root->GetChildCount(); i++)
			{
				FbxNode * node = root->GetChild(i);
				if (node != nullptr)
				{
					search_and_load_fbx(scene, node, ret);
				}
			}

			return;
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

		Mesh * load_fbx(const std::string& file)
		{
			FbxManager* manager = FbxManager::Create();

			FbxIOSettings * ios_setting = FbxIOSettings::Create(manager, IOSROOT);
			manager->SetIOSettings(ios_setting);

			FbxImporter* importer = FbxImporter::Create(manager, "");

			if (!importer->Initialize(file.c_str(), -1, manager->GetIOSettings()))
			{
				return nullptr;
			}

			FbxScene* scene = FbxScene::Create(manager, "");
			importer->Import(scene);

			FbxAxisSystem axis(FbxAxisSystem::eYAxis, FbxAxisSystem::eParityOdd, FbxAxisSystem::eRightHanded);
			axis.ConvertScene(scene);

			triangulate_recursive(scene, scene->GetRootNode());

			Mesh * ret = new Mesh;
			search_and_load_fbx(scene, scene->GetRootNode(), ret);
			manager->Destroy();

			return ret;
		}*/
	}
}
