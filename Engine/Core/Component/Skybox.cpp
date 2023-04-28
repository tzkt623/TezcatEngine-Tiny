#include "Skybox.h"
#include "GameObject.h"

#include "../Data/Material.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"
#include "../Manager/PipelineManager.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Vertex.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderLayer.h"

#include "../Manager/VertexManager.h"

namespace tezcat::Tiny
{
	TINY_RTTI_CPP(Skybox)

	Skybox::Skybox()
		: mVertex(VertexMgr::getInstance()->getVertex("Skybox"))
		, mDrawMode(ContextMap::DrawModeArray[(int)DrawMode::Triangles])
		, mMaterial(nullptr)
		, mRenderAgent(mRenderAgent = RenderAgent::create(this, this))
	{
		mVertex->addRef();
		mRenderAgent->addRef();
	}

	Skybox::~Skybox()
	{
		mVertex->subRef();
		mRenderAgent->subRef();
		mMaterial->subRef();
	}

	void Skybox::onStart()
	{
		RenderLayer::addRenderAgent(this->getGameObject()->getLayerIndex(), mRenderAgent);
	}

	void Skybox::onDisable()
	{

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
		return mVertex->getVertexCount();
	}

	void Skybox::beginRender()
	{
		mVertex->bind();
	}

	void Skybox::endRender()
	{
		mVertex->unbind();
	}

	void Skybox::setMaterial(Material* material)
	{
		if (mMaterial)
		{
			mMaterial->subRef();
		}

		mMaterial = material;
		mMaterial->addRef();
	}

}
