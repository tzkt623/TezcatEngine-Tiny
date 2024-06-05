#include "GLRenderCommand.h"
#include "GLHead.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Texture.h"
#include "Core/Component/Transform.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Data/Material.h"
#include "Core/Manager/ShadowCasterManager.h"
#include "Core/Manager/LightingManager.h"

namespace tezcat::Tiny::GL
{
	//-------------------------------------------------------
	//
	//	GLRenderCMD_Vertex
	//
	GLRenderCMD_Vertex::GLRenderCMD_Vertex(Vertex* vertex)
		: RenderCommand(vertex, nullptr, nullptr)
	{

	}

	GLRenderCMD_Vertex::~GLRenderCMD_Vertex()
	{

	}

	void GLRenderCMD_Vertex::run(Shader* shader)
	{
		Graphics::getInstance()->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	GLRenderCMD_Mesh
	//
	GLRenderCMD_Mesh::~GLRenderCMD_Mesh()
	{

	}

	GLRenderCMD_Mesh::GLRenderCMD_Mesh(Vertex* vertex, Transform* transform, Material* material)
		: RenderCommand(vertex, transform, material)
	{

	}

	void GLRenderCMD_Mesh::run(Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, model_mat4);

		glm::mat3 normal_matrix(model_mat4);
		Graphics::getInstance()->setMat3(shader, ShaderParam::MatrixN, glm::inverseTranspose(normal_matrix));

		mMaterial->submit(shader);
		Graphics::getInstance()->draw(mVertex);
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_Shadow
	//
	GLRenderCMD_Shadow::~GLRenderCMD_Shadow()
	{

	}

	GLRenderCMD_Shadow::GLRenderCMD_Shadow(Vertex* vertex, Transform* transform)
		: RenderCommand(vertex, transform, nullptr)
	{

	}

	void GLRenderCMD_Shadow::run(Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, model_mat4);
		Graphics::getInstance()->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_Skybox
	//
	GLRenderCMD_Skybox::~GLRenderCMD_Skybox()
	{

	}

	GLRenderCMD_Skybox::GLRenderCMD_Skybox(Vertex* vertex
		, TextureCube* skybox
		, float lod
		, bool isHdr
		, float exposure)
		: RenderCommand(vertex, nullptr, nullptr)
		, mSkybox(skybox)
		, mLod(lod)
		, mIsHdr(isHdr)
		, mExposure(exposure)
	{

	}

	void GLRenderCMD_Skybox::run(Shader* shader)
	{
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, mSkybox);
		Graphics::getInstance()->setFloat1(shader, "myLod", mLod);
		Graphics::getInstance()->setBool(shader, "myIsHDR", mIsHdr);
		Graphics::getInstance()->setFloat1(shader, "myExposure", mExposure);
		Graphics::getInstance()->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_HDRToCube
	//
	GLRenderCMD_HDRToCube::GLRenderCMD_HDRToCube(Vertex* vertex, Texture2D* texHDR, TextureCube* skybox)
		: RenderCommand(vertex, nullptr, nullptr)
		, mTexHDR(texHDR)
		, mSkybox(skybox)
	{

	}

	GLRenderCMD_HDRToCube::~GLRenderCMD_HDRToCube()
	{

	}

	void GLRenderCMD_HDRToCube::run(Shader* shader)
	{
		float4x4 captureViews[] =
		{
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(1.0f,  0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(-1.0f, 0.0f,  0.0f), float3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  1.0f,  0.0f), float3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f, -1.0f,  0.0f), float3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f,  1.0f), float3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(float3(0.0f, 0.0f, 0.0f), float3(0.0f,  0.0f, -1.0f), float3(0.0f, -1.0f,  0.0f))
		};

		shader->resetLocalState();
		auto uinfo = shader->getUniformInfo("myTexHDR2D");
		Graphics::getInstance()->setTexture2D(shader, uinfo->shaderID, mTexHDR);

		Texture2D** array = LightingManager::getCubeMapTextureArray();

		for (uint32 i = 0; i < 6; i++)
		{
			Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				 , GL_COLOR_ATTACHMENT0
				 , GL_TEXTURE_2D
				 , array[i]->getTextureID()
				 , 0);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(mVertex);

			glFramebufferTexture2D(GL_FRAMEBUFFER
				, GL_COLOR_ATTACHMENT0
				, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
				, mSkybox->getTextureID()
				, 0);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(mVertex);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, mSkybox->getTextureID());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	//-------------------------------------------------------
	//
	//	GLRenderCMD_EnvMakeIrradiance
	//
	GLRenderCMD_EnvMakeIrradiance::GLRenderCMD_EnvMakeIrradiance(Vertex* vertex, TextureCube* skybox, TextureCube* irradiance)
		: RenderCommand(vertex, nullptr, nullptr)
		, mSkyboxMap(skybox)
		, mIrradianceMap(irradiance)
	{

	}

	GLRenderCMD_EnvMakeIrradiance::~GLRenderCMD_EnvMakeIrradiance()
	{

	}

	void GLRenderCMD_EnvMakeIrradiance::run(Shader* shader)
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
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, mSkyboxMap);

		for (uint32 i = 0; i < 6; i++)
		{
			//mFrameBuffer->attachCube(mIrradianceMap, 0, i);
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , mIrradianceMap->getTextureID()
								 , 0);

			Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);
			//shader->setViewMatrix(captureViews[i]);

			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(mVertex);
		}
	}

	//-------------------------------------------------------
	//
	//	GLRenderCMD_EnvMakePrefilter
	//
	GLRenderCMD_EnvMakePrefilter::GLRenderCMD_EnvMakePrefilter(Vertex* vertex
		, TextureCube* skybox
		, TextureCube* prefitler
		, uint32 mipMaxLevel
		, uint32 mipWidth
		, uint32 mipHeight
		, float resolution)
		: RenderCommand(vertex, nullptr, nullptr)
		, mSkyboxMap(skybox)
		, mPrefilterMap(prefitler)
		, mMipMaxLevel(mipMaxLevel)
		, mMipWidth(mipWidth)
		, mMipHeight(mipHeight)
		, mResolution(resolution)
	{

	}

	GLRenderCMD_EnvMakePrefilter::~GLRenderCMD_EnvMakePrefilter()
	{

	}

	void GLRenderCMD_EnvMakePrefilter::run(Shader* shader)
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
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, mSkyboxMap);

		for (uint32 mip = 0; mip < mMipMaxLevel; ++mip)
		{
			uint32 mip_width = static_cast<uint32>(mMipWidth * std::pow(0.5, mip));
			uint32 mip_height = static_cast<uint32>(mMipHeight * std::pow(0.5, mip));
			Graphics::getInstance()->setViewport(ViewportInfo(0, 0, mip_width, mip_height));

			float roughness = (float)mip / (float)(mMipMaxLevel - 1);
			Graphics::getInstance()->setFloat1(shader, "myRoughness", &roughness);
			Graphics::getInstance()->setFloat1(shader, "myResolution", &mResolution);

			for (uint32 i = 0; i < 6; ++i)
			{
				//mFrameBuffer->attachCube(mPrefilterMap, 0, i, mip);
				glFramebufferTexture2D(GL_FRAMEBUFFER
									 , GL_COLOR_ATTACHMENT0
									 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
									 , mPrefilterMap->getTextureID()
									 , mip);

				Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);

				Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
				Graphics::getInstance()->draw(mVertex);
			}
		}

		//glDisable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	}
}
