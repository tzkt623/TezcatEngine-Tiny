#include "MyGUIContext.h"
#include "Window/MyObjectInfoWindow.h"
#include "../MyInputer.h"

namespace tezcat::Editor
{
	const ImVec2 MyGUIContext::UV0(0, 1);
	const ImVec2 MyGUIContext::UV1(1, 0);
	ImVec2 MyGUIContext::sViewPortSize;
	GameObject* MyGUIContext::sSelectedGameObject;

	MyGUIContext::MyGUIContext()
		: mValueConfigAry(UniformID::allStringCount(), nullptr)
	{
		this->initValueConfig();
	}


	MyGUIContext::~MyGUIContext()
	{

	}


	ValueConfig* MyGUIContext::getValueConfig(const UniformID& ID)
	{
		return mValueConfigAry[ID.toUID()];
	}

	void MyGUIContext::initValueConfig()
	{
		mValueConfigAry[ShaderParam::LightDirection::Ambient.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Diffuse.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Specular.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
	}


	//-------------------------------------------------------------
	//
	//
	//
	//
	void MyGUIContext::matrix4(glm::mat4& mat4)
	{
		ImGui::Text("x");
		ImGui::SameLine();
		ImGui::InputFloat4("##x", glm::value_ptr(mat4[0]));

		ImGui::Text("y");
		ImGui::SameLine();
		ImGui::InputFloat4("##y", glm::value_ptr(mat4[1]));

		ImGui::Text("z");
		ImGui::SameLine();
		ImGui::InputFloat4("##z", glm::value_ptr(mat4[2]));

		ImGui::Text("w");
		ImGui::SameLine();
		ImGui::InputFloat4("##w", glm::value_ptr(mat4[3]));
	}

	void MyGUIContext::transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
	{
		ImGui::Text("Position");
		ImGui::SameLine();
		ImGui::DragFloat3("##Position", &position.x);

		ImGui::Text("Rotation");
		ImGui::SameLine();
		ImGui::DragFloat3("##Rotation", &rotation.x);

		ImGui::Text("Scale");
		ImGui::SameLine();
		ImGui::DragFloat3("##Scale", &scale.x);
	}

	void MyGUIContext::clearOnPopScene()
	{
		MyGUIContext::sSelectedGameObject = nullptr;
	}

	file_path MyGUIContext::openFile(const char* filter)
	{
		file_path path;
		//TINY_LOG_ENGINE(ResourceManager::getRelativeResDir().c_str());
		nfdchar_t* outPath = nullptr;
		nfdresult_t result = NFD_OpenDialog(filter
			, nullptr
			, &outPath);

		if (result == NFD_OKAY)
		{
			path = outPath;
			//puts("Success!");
			//puts(outPath);
			free(outPath);
		}
		else if (result == NFD_CANCEL)
		{
			TINY_LOG_ENGINE("User pressed cancel.");
			//puts("User pressed cancel.");
		}
		else
		{
			TINY_LOG_ENGINE(NFD_GetError());
			//printf("Error: %s\n", NFD_GetError());
		}

		return path;
	}

	void MyGUIContext::createCamera()
	{
		if (SceneManager::empty())
		{
			return;
		}

		if (CameraManager::getMainCamera())
		{

		}
		else
		{
			InputSys::getInstance()->push(MyInputer::getInstance());

			auto go = GameObject::create("MainCamera");
			go->addComponent<Transform>()->setPosition(float3(0.0f, 0.0f, 10.0f));

			auto camera = go->addComponent<Camera>();
			camera->setMain();
			camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
			camera->setPerspective(60.0f, 0.1f, 2000.0f);
			camera->setCullLayer(0);
			camera->setClearOption(ClearOption::CO_Skybox | ClearOption::CO_Depth | ClearOption::CO_Color);
			camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());

			MyInputer::getInstance()->setController(go->addComponent<FlyController>());
		}
	}

	void MyGUIContext::createBuildinModel(const std::string& model)
	{
		if (SceneManager::empty())
		{
			return;
		}

		auto shader = ShaderManager::find("Standard/StdValue1");

		auto index_diffuse = shader->getUniformIndex("myDiffuse");
		auto index_specular = shader->getUniformIndex("mySpecular");
		auto index_shininess = shader->getUniformIndex("myShininess");

		auto go = GameObject::create(model);
		go->addComponent<Transform>();
		auto mr = go->addComponent<MeshRenderer>();
		mr->setMesh(model);

		auto material = Material::create(shader);
		mr->setMaterial(material);

		material->setUniform<UniformF3>(index_diffuse, float3(1.0f));
		material->setUniform<UniformF3>(index_specular, float3(64.0f));
		material->setUniform<UniformF1>(index_shininess, 0.0f);
	}

	void MyGUIContext::createDirectionLight()
	{
		if (SceneManager::empty())
		{
			return;
		}

		if (LightingManager::hasDirectionalLight())
		{
			return;
		}

		auto go = GameObject::create("DirectionLight");
		go->addComponent<Transform>();
		go->getTransform()->setPosition(float3(0.0f, 0.0f, 0.0f));
		//go->getTransform()->setScale(float3(100.0f));
		go->getTransform()->setRotation(-60.0f, 0.0f, 0.0f);

		//auto mr = go->addComponent<MeshRenderer>();
		//mr->setMesh("Sphere");
		//
		//auto light_material = Material::create("Unlit/Color");
		//mr->setMaterial(light_material);

		auto dir_light = go->addComponent<DirectionalLight>();
		dir_light->setDiffuse(float3(1.0f, 1.0f, 1.0f));
		dir_light->setAmbient(float3(0.1f));
		dir_light->setSpecular(float3(0.5f));

		auto shadow_caster = go->addComponent<ShadowCaster>();
		shadow_caster->setOrtho(0.1f, 2000.0f);
		shadow_caster->setViewRect(0, 0, 1024, 1024);
		shadow_caster->setCullLayer(0);
		shadow_caster->setShadowMap(4096, 4096, "Shadow");
	}
}
