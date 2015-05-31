#include "SceneLoader.h"
#include "../../scene/Scene.h"
#include "../../scene/node.h"
#include "../../scene/Entity.h"
#include "../../scene/Light.h"
#include "../../scene/Camera.h"
#include <json/json.h>
#include "../../util/FileUtil.h"
#include "../../System.h"

namespace loader {
	SceneLoader::SceneLoader()
	{
	}


	SceneLoader::~SceneLoader()
	{
	}

	enum NodeType {
		NONE = 0,
		NODE = 1,
		ENTITY = 2,
		LIGHT = 3,
		CAMERA = 4,
	};

	lly::Node * parse_normal(const Json::Value& root)
	{
		lly::Node * ret = new lly::Node;
		return ret;
	}

	lly::Node * parse_entity(const Json::Value& root)
	{
		lly::Entity * ret = new lly::Entity;
		auto file = root["path"].asString();
		auto mesh = lly::System::instance().get_resource_manager().load_mesh("zhaoyun_.fbx");
		auto mesh_ptr = lly::System::instance().get_resource_manager().get_mesh(mesh);
		ret->add_mesh(mesh_ptr);

		ret->play_animation("Take 001", true);

		return ret;
	}

	lly::Node * parse_light(const Json::Value& root)
	{
		lly::Light * ret = new lly::Light;
		auto color = root["color"];
		ret->set_color(glm::vec4(color["r"].asFloat(), color["g"].asFloat(), color["b"].asFloat(), color["a"].asFloat()));

		auto type = root["light_type"].asInt();
		switch (type)
		{
		case 0: 
			{
				ret->set_type(lly::Light::Type::AMBIENT);
			}
			break;
		case 1:
			{
				ret->set_type(lly::Light::Type::DIRECTIONAL);
				auto direction = root["direction"];
				ret->set_direction(glm::vec3(direction["x"].asFloat(), direction["y"].asFloat(), direction["z"].asFloat()));
			}
			break;
		case 2:
			{
				ret->set_type(lly::Light::Type::POINT);
				ret->set_constant(root["constant"].asFloat());
				ret->set_linear(root["linear"].asFloat());
				ret->set_quadratic(root["quadratic"].asFloat());
			}
			break;
		case 3:
			{
				ret->set_type(lly::Light::Type::SPOT);
				auto direction = root["direction"];
				ret->set_direction(glm::vec3(color["x"].asFloat(), color["y"].asFloat(), color["z"].asFloat()));
				ret->set_constant(root["constant"].asFloat());
				ret->set_linear(root["linear"].asFloat());
				ret->set_quadratic(root["quadratic"].asFloat());
				ret->set_exp(root["exp"].asFloat());
				ret->set_cutoff(root["cutoff"].asFloat());
			}
			break;
		default:
			throw std::logic_error("unsupport light type");
		}
		return ret;
	}

	lly::Node * parse_camera(const Json::Value& root)
	{
		lly::Camera * ret = new lly::Camera;
		ret->set_order(root["order"].asInt());
		auto n = root["near"].asFloat();
		auto f = root["far"].asFloat();

		auto type = root["camera_type"].asInt();
		switch (type)
		{
		case 0:
			{
				auto fovy = root["fovy"].asFloat();
				auto aspect = root["aspect"].asFloat();
				ret->set_prespective(fovy, aspect, n, f);
			}
			break;
		case 1:
			{
				auto bottom = root["bottom"].asFloat();
				auto top = root["top"].asFloat();
				auto right = root["right"].asFloat();
				auto left = root["left"].asFloat();
				ret->set_ortho(left, right, top, bottom, n, f);
			}
			break;
		default:
			throw std::logic_error("unsupport camera type");
		}
		return ret;
	}

	lly::Node * parse_scene_node(const Json::Value& root)
	{
		lly::Node * ret = nullptr;
		auto type = root["type"].asInt();
		switch (type)
		{
		case NodeType::NODE: ret = parse_normal(root); break;
		case NodeType::ENTITY: ret = parse_entity(root);  break;
		case NodeType::LIGHT: ret = parse_light(root); break;
		case NodeType::CAMERA: ret = parse_camera(root); break;
		default: 
			throw std::logic_error("unsupport node type");
			break;
		}

		ret->set_name(root["name"].asString());
		auto position = root["position"];
		ret->set_position(position["x"].asFloat(), position["y"].asFloat(), position["z"].asFloat());

		auto rotation = root["rotation"];
		ret->set_rotation(rotation["x"].asFloat() * M_PI / 180.0f, rotation["y"].asFloat() * M_PI / 180.0f, rotation["z"].asFloat() * M_PI / 180.0f);

		auto scale = root["scale"];
		ret->set_scale(scale["x"].asFloat(), scale["y"].asFloat(), scale["z"].asFloat());

		auto node_count = root["children"].size();
		for (int i = 0; i < node_count; ++i)
		{
			lly::Node * node = parse_scene_node(root["children"][i]);
			ret->add_child(node);
			node->set_parent(ret);
		}

		return ret;
	}

	lly::Scene * SceneLoader::load_from(const std::string& file)
	{
		lly_util::Data data = lly_util::load_from_file(file);

		Json::Reader reader;
		Json::Value root;
		if (!reader.parse(data.data(), root))
		{
			throw std::logic_error("load scene file failed.");
		}

		lly::Scene * scene = new lly::Scene;
		auto node_count = root["nodes"].size();
		for (int i = 0; i < node_count; ++i)
		{
			lly::Node * node = parse_scene_node(root["nodes"][i]);
			scene->add_node(node);

			node->visit([&scene](lly::Node * node) {
				node->add_to_scene(scene);
			});
		}

		return scene;
	}
}
