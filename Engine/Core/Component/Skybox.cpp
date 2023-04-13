#include "Skybox.h"
#include "GameObject.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/VertexGroup.h"
#include "../Renderer/BaseGraphics.h"
#include "../Layer/RenderLayer.h"

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
		RenderLayer::addRenderObejct(this->getGameObject()->getLayerIndex(), this);
	}

	void Skybox::onEnable()
	{
	}

	Material* Skybox::getMaterial() const
	{
		return mMaterial;
	}

	void Skybox::sendToRenderPass(const RenderPassType& passType)
	{
		if (passType != RenderPassType::Default)
		{
			return;
		}
		auto shader = mMaterial->getShaderPackage()->getShaders()[0];
		RenderPass::get(shader)->addRenderMesh(this);
	}

	void Skybox::submit(Shader* shader)
	{
		mMaterial->submit(this->getTransform(), shader);
	}

	DrawModeWrapper& Skybox::getDrawMode()
	{
		return mDrawMode;
	}

	int Skybox::getVertexCount() const
	{
		return mVertexGroup->getVertexCount();
	}

	void Skybox::beginRender()
	{
		mVertexGroup->bind();
	}

	void Skybox::endRender()
	{
		mVertexGroup->unbind();
	}
}
