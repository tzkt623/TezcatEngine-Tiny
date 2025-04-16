/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "EditorContext.h"
#include "Window/EditorObjectInfoWindow.h"
#include "../EditorCamera.h"
#include "../EditorInputer.h"

namespace tezcat::Editor
{
	const ImVec2 EditorContext::UV0(0, 1);
	const ImVec2 EditorContext::UV1(1, 0);
	ImVec2 EditorContext::sViewPortSize;
	GameObject* EditorContext::SelectedGameObject;
	std::vector<GameObject*> EditorContext::DeleteArray;
	ImGuizmo::MODE EditorContext::SelectedObjectMode = ImGuizmo::MODE::LOCAL;
	ImGuizmo::OPERATION EditorContext::SelectedObjectOperation = ImGuizmo::OPERATION::TRANSLATE;

	FrameBuffer* EditorContext::EditorFrameBuffer = nullptr;
	EditorCamera* EditorContext::EditorCamera = nullptr;
	Texture2D* EditorContext::EditorTex2DColor = nullptr;
	std::vector<ValueConfig*> EditorContext::mValueConfigAry;

	bool EditorContext::IsPause = false;
	bool EditorContext::IsPlaying = false;
	bool EditorContext::IsFocusOnGameView = false;
	bool EditorContext::EditorSceneFoucsed = false;

	void EditorContext::init()
	{
		EngineEvent::getInstance()->addListener(EngineEventID::EE_AfterSceneEnter, EditorFrameBuffer,
			[](const EventData& data)
			{

			});

		EngineEvent::getInstance()->addListener(EngineEventID::EE_AfterSceneExit, EditorFrameBuffer,
			[](const EventData& data)
			{
				EditorTex2DColor->clearInGPU();
			});

		mValueConfigAry.resize(UniformID::allStringCount(), nullptr);
		mValueConfigAry[ShaderParam::LightDirection::Ambient.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Diffuse.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };
		mValueConfigAry[ShaderParam::LightDirection::Specular.toUID()] = new ValueConfig{ true, 0.005f, 0.0f, 1.0f };

		auto [flag, fb] = FrameBufferManager::create("FB_Editor");
		EditorFrameBuffer = fb;
		EditorFrameBuffer->saveObject();

		EditorTex2DColor = Texture2D::create("T2D_Tiny_Editor_Color");
		EditorTex2DColor->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
				, TextureInternalFormat::RGBA
				, TextureFormat::RGBA);
		EditorTex2DColor->setAttachPosition(TextureAttachPosition::ColorComponent);

		auto tex_depth = Texture2D::create("T2D_Tiny_Editor_Depth");
		tex_depth->setConfig(Engine::getScreenWidth(), Engine::getScreenHeight()
			, TextureInternalFormat::Depth
			, TextureFormat::Depth);
		tex_depth->setAttachPosition(TextureAttachPosition::DepthComponent);

		EditorFrameBuffer->addAttachment(EditorTex2DColor);
		EditorFrameBuffer->addAttachment(tex_depth);
		EditorFrameBuffer->generate();

		EditorCamera = EditorCamera::create();
		EditorCamera->setPerspective(60, 0.1, 2000);
		EditorCamera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
		EditorCamera->setSortingID(-100);
		EditorCamera->setClearOption(ClearOption::CO_Skybox | ClearOption::CO_Depth | ClearOption::CO_Color);
		EditorCamera->setFrameBuffer(EditorFrameBuffer);
		for (int32_t i = 0; i < 32; i++)
		{
			EditorCamera->addCullLayer(i);
		}
		EditorCamera->saveObject();

		auto transform = Transform::create();
		//transform->setPosition(float3(-41.715f, 174.206f, 382.644f));
		//transform->setRotation(float3(-13.093f, -27.649f, 0.0f));

		EditorCamera->setTransform(transform);

		GameObjectManager::setIDObserver(EditorCamera);
	}

	ValueConfig* EditorContext::getValueConfig(const UniformID& ID)
	{
		return mValueConfigAry[ID.toUID()];
	}


	//-------------------------------------------------------------
	//
	//
	//
	//
	void EditorContext::matrix4(glm::mat4& mat4)
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

	void EditorContext::transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
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

	void EditorContext::clearOnPopScene()
	{
		EditorContext::SelectedGameObject = nullptr;
	}

	file_path EditorContext::openFile(const char* filter)
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

	void EditorContext::createCamera()
	{
		if (SceneManager::isEmpty())
		{
			return;
		}

		if (CameraManager::isDataValied())
		{
			//InputSys::getInstance()->push(EditorInputer::getInstance());

			auto go = GameObject::create("Camera");
			go->addComponent<Transform>()->setPosition(float3(0.0f, 0.0f, 10.0f));

			auto camera = go->addComponent<Camera>();
			camera->setViewRect(0, 0, Engine::getScreenWidth(), Engine::getScreenHeight());
			camera->setPerspective(60.0f, 0.1f, 2000.0f);
			camera->setCullLayer(0);
			camera->setClearOption(ClearOption::CO_Skybox | ClearOption::CO_Depth | ClearOption::CO_Color);
			camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());

			//EditorInputer::getInstance()->setController(go->addComponent<FlyController>());
		}
		else
		{

		}
	}

	void EditorContext::createBuildinModel(const std::string& model)
	{
		if (SceneManager::isEmpty())
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

	void EditorContext::createDirectionLight()
	{
		if (SceneManager::isEmpty())
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

	void EditorContext::endFrame()
	{
		if (!DeleteArray.empty())
		{
			for (auto go : DeleteArray)
			{
				go->deleteObject();
			}

			DeleteArray.clear();
		}

		if (EditorContext::EditorSceneFoucsed && EditorContext::EditorCamera)
		{
			EditorContext::EditorCamera->getTransform()->update();
			EditorContext::EditorCamera->preRender();

			//先剔除
			for (auto& index : EditorContext::EditorCamera->getCullLayerList())
			{
				//剔除到对应的渲染通道
				RenderObjectCache::culling(index, EditorContext::EditorCamera);
			}

			EditorContext::EditorCamera->getPipelineQueue()->addToPipeline();
		}

		EditorContext::EditorSceneFoucsed = false;
	}

	void EditorContext::beginFrame()
	{

	}


}
