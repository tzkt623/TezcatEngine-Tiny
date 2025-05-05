/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Tutorial_Skybox.h"
#include "../EditorInputer.h"

using namespace tezcat::Editor;

TINY_OBJECT_CPP(Tutorial_Skybox, Scene);

Tutorial_Skybox::Tutorial_Skybox(const std::string& name)
	: Base(name)
{

}

Tutorial_Skybox::~Tutorial_Skybox()
{

}

void Tutorial_Skybox::onEnter()
{
	Base::onEnter();
	InputSys::getInstance()->push(EditorInputer::getInstance());
	LightingManager::loadSkyboxHDR("Resource/Image/SkyboxHDR/newport_loft.hdr");

	GameObject* go_camera = GameObject::create("MainCamera");
	auto transform = go_camera->addComponent<Transform>();
	transform->setPosition(0.0f, 15.0f, 30.0f);
	transform->setRotation(-20.0f, 0.0f, 0.0f);

	auto camera = go_camera->addComponent<Camera>();
	camera->setPerspective(75.0f, 0.1f, 2000.0f);
	camera->setClearColor(float4(0.3f, 0.3f, 0.3f, 1.0f));
	camera->setClearOption({ ClearOption::CO_Color | ClearOption::CO_Depth | ClearOption::CO_Skybox });
	camera->setCullLayer(0);
	camera->setViewRect(0, 0, EngineContext::ScreenWidth, EngineContext::ScreenHeight);
	camera->setFrameBuffer(FrameBufferManager::getMainFrameBufferBuildin());
	camera->setMain();

	go_camera->addComponent<FlyController>();
}

void Tutorial_Skybox::onExit()
{
	Base::onExit();
}

