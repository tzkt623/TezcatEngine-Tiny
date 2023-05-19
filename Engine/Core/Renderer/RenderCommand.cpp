#include "RenderCommand.h"
#include "../Shader/Shader.h"
#include "../Shader/ShaderPackage.h"

#include "../Renderer/Vertex.h"
#include "../Renderer/BaseGraphics.h"
#include "../Renderer/RenderPass.h"
#include "../Renderer/Texture.h"
#include "../Renderer/FrameBuffer.h"

#include "../Component/Transform.h"

#include "../Manager/ShadowCasterManager.h"

#include "../Data/Material.h"

namespace tezcat::Tiny
{
	RenderCommand::RenderCommand(Shader* shader)
		: mShader(shader)
		, mOrderID(0)
	{

	}

	RenderCommand::~RenderCommand()
	{
		mShader = nullptr;
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_Vertex
	//
	RenderCMD_Vertex::RenderCMD_Vertex(Vertex* vertex, Shader* shader)
		: RenderCommand(shader)
		, mVertex(vertex)
	{

	}

	RenderCMD_Vertex::~RenderCMD_Vertex()
	{

	}

	void RenderCMD_Vertex::draw(BaseGraphics* graphics, Shader* shader)
	{
		graphics->draw(mVertex);
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_Shadow
	//
	RenderCMD_Shadow::~RenderCMD_Shadow()
	{

	}

	RenderCMD_Shadow::RenderCMD_Shadow(Vertex* vertex, Transform* transform)
		: RenderCommand(ShadowCasterMgr::getInstance()->getShader())
		, mVertex(vertex)
		, mTransform(transform)
	{

	}

	void RenderCMD_Shadow::draw(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		shader->setMat4(ShaderParam::MatrixM, model_mat4);
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_Skybox
	//
	RenderCMD_Skybox::~RenderCMD_Skybox()
	{

	}

	RenderCMD_Skybox::RenderCMD_Skybox(Vertex* vertex, Transform* transform, Material* material)
		: RenderCommand(material->getShaderPackage()->getShaders()[0])
		, mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	void RenderCMD_Skybox::draw(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		shader->setMat4(ShaderParam::MatrixM, model_mat4);

		mMaterial->submit(shader);
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_Mesh
	//
	RenderCMD_Mesh::~RenderCMD_Mesh()
	{

	}

	RenderCMD_Mesh::RenderCMD_Mesh(Vertex* vertex, Transform* transform, Material* material)
		: RenderCommand(material->getShaderPackage()->getShaders()[0])
		, mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	void RenderCMD_Mesh::draw(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		shader->setMat4(ShaderParam::MatrixM, model_mat4);
		glm::mat3 normal_matrix(model_mat4);
		shader->setMat3(ShaderParam::MatrixN, glm::inverseTranspose(normal_matrix));

		mMaterial->submit(shader);
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_HDRToCube
	//
	RenderCMD_HDRToCube::RenderCMD_HDRToCube(Vertex* vertex, Texture2D* hdr, TextureCube* cube, Shader* shader, FrameBuffer* frameBuffer)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mHDR(hdr)
		, mCube(cube)
		, mFrameBuffer(frameBuffer)
	{

	}

	RenderCMD_HDRToCube::~RenderCMD_HDRToCube()
	{

	}

	void RenderCMD_HDRToCube::draw(BaseGraphics* graphics, Shader* shader)
	{
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		shader->resetLocalState();
		shader->setTexture2D(ShaderParam::TexColor, mHDR);

		for (uint32_t i = 0; i < 6; i++)
		{
			mFrameBuffer->attachCube(mCube, 0, i);
			shader->setViewMatrix(captureViews[i]);

			graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			graphics->draw(mVertex);
		}

		mCube->bind();
		mCube->genMipmap();
		mCube->unbind();
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_EnvMakeIrradiance
	//
	RenderCMD_EnvMakeIrradiance::RenderCMD_EnvMakeIrradiance(Vertex* vertex, TextureCube* cube, TextureCube* irradiance, Shader* shader, FrameBuffer* frameBuffer)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mCubeMap(cube)
		, mIrradianceMap(irradiance)
		, mFrameBuffer(frameBuffer)
	{

	}

	RenderCMD_EnvMakeIrradiance::~RenderCMD_EnvMakeIrradiance()
	{

	}

	void RenderCMD_EnvMakeIrradiance::draw(BaseGraphics* graphics, Shader* shader)
	{
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		shader->resetLocalState();
		shader->setTextureCube(ShaderParam::TexCube, mCubeMap);

		for (uint32_t i = 0; i < 6; i++)
		{
			mFrameBuffer->attachCube(mIrradianceMap, 0, i);
			shader->setViewMatrix(captureViews[i]);

			graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			graphics->draw(mVertex);
		}
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_EnvMakePrefilter
	//
	RenderCMD_EnvMakePrefilter::RenderCMD_EnvMakePrefilter(Vertex* vertex
		, TextureCube* cube
		, TextureCube* prefitler
		, Shader* shader
		, FrameBuffer* frameBuffer
		, uint32_t mipMaxLevel
		, uint32_t mipWidth
		, uint32_t mipHeight
		, float resolution)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mCubeMap(cube)
		, mPrefilterMap(prefitler)
		, mFrameBuffer(frameBuffer)
		, mMipMaxLevel(mipMaxLevel)
		, mMipWidth(mipWidth)
		, mMipHeight(mipHeight)
		, mResolution(resolution)

	{

	}

	RenderCMD_EnvMakePrefilter::~RenderCMD_EnvMakePrefilter()
	{

	}

	void RenderCMD_EnvMakePrefilter::draw(BaseGraphics* graphics, Shader* shader)
	{
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, 0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		shader->resetLocalState();
		shader->setTextureCube(ShaderParam::TexCube, mCubeMap);

		for (uint32_t mip = 0; mip < mMipMaxLevel; ++mip)
		{
			uint32_t mip_width = static_cast<uint32_t>(mMipWidth * std::pow(0.5, mip));
			uint32_t mip_height = static_cast<uint32_t>(mMipHeight * std::pow(0.5, mip));
			graphics->setViewport(ViewportInfo(0, 0, mip_width, mip_height));

			float roughness = (float)mip / (float)(mMipMaxLevel - 1);
			shader->setFloat1("roughness", &roughness);
			shader->setFloat1("resolution", &mResolution);

			for (uint32_t i = 0; i < 6; ++i)
			{
				mFrameBuffer->attachCube(mPrefilterMap, 0, i, mip);
				shader->setViewMatrix(captureViews[i]);

				graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
				graphics->draw(mVertex);
			}
		}
	}
}
