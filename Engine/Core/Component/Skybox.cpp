#include "Skybox.h"
#include "GameObject.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexGroup.h"
#include "../Renderer/BaseGraphics.h"

#include "../Manager/VertexGroupManager.h"

namespace tezcat::Tiny::Core
{
	Skybox::Skybox()
		: mVertexGroup(VertexGroupMgr::getInstance()->getVertexGroup("Skybox"))
		, mDrawMode(ContextMap::DrawModeArray[(int)DrawMode::Triangles])
		, mMaterial(nullptr)
	{

	}

	Skybox::~Skybox()
	{
		delete mMaterial;

		mVertexGroup = nullptr;
		mMaterial = nullptr;
	}

	void Skybox::onStart()
	{

	}

	void Skybox::onEnable()
	{
		PipelineManager::addRenderObject(this->getGameObject()->getLayerMaskIndex(), this);
	}

	Material* Skybox::getMaterial() const
	{
		return mMaterial;
	}

	void Skybox::sendToRenderPass()
	{
		auto shader = mMaterial->getShaderPackage()->getShaders()[0];
		PipelineManager::getPass(shader->getUID())->addRenderObject(this);
	}

	void Skybox::submit(Shader* shader)
	{
		mVertexGroup->bind();
		mMaterial->submit(this->getTransform(), shader);
		Graphics::getInstance()->draw(this);
		mVertexGroup->unbind();
	}

	DrawModeWrapper& Skybox::getDrawMode()
	{
		return mDrawMode;
	}

	int Skybox::getVertexCount() const
	{
		return mVertexGroup->getVertexCount();
	}
}
