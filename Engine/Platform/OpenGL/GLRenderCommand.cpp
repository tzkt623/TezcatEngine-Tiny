#include "GLRenderCommand.h"
#include "GLHead.h"

#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/Texture.h"
#include "Core/Component/Transform.h"
#include "Core/Shader/Shader.h"
#include "Core/Shader/ShaderPackage.h"
#include "Core/Data/Material.h"
#include "Core/Manager/ShadowCasterManager.h"

namespace tezcat::Tiny::GL
{
	//-------------------------------------------------------
	//
	//	GLRenderCMD_Vertex
	//
	GLRenderCMD_Vertex::GLRenderCMD_Vertex(Shader* shader, Vertex* vertex)
		: RenderCommand(shader)
		, mVertex(vertex)
	{

	}

	GLRenderCMD_Vertex::~GLRenderCMD_Vertex()
	{

	}

	void GLRenderCMD_Vertex::run(BaseGraphics* graphics, Shader* shader)
	{
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	GLRenderCMD_Mesh
	//
	GLRenderCMD_Mesh::~GLRenderCMD_Mesh()
	{

	}

	GLRenderCMD_Mesh::GLRenderCMD_Mesh(Vertex* vertex, Transform* transform, Material* material)
		: RenderCommand(material->getShader())
		, mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	void GLRenderCMD_Mesh::run(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		graphics->setMat4(shader, ShaderParam::MatrixM, model_mat4);

		glm::mat3 normal_matrix(model_mat4);
		graphics->setMat3(shader, ShaderParam::MatrixN, glm::inverseTranspose(normal_matrix));

		mMaterial->submit(graphics, shader);
		graphics->draw(mVertex);
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_Shadow
	//
	GLRenderCMD_Shadow::~GLRenderCMD_Shadow()
	{

	}

	GLRenderCMD_Shadow::GLRenderCMD_Shadow(Vertex* vertex, Transform* transform)
		: RenderCommand(ShadowCasterMgr::getInstance()->getShader())
		, mVertex(vertex)
		, mTransform(transform)
	{

	}

	void GLRenderCMD_Shadow::run(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		graphics->setMat4(shader, ShaderParam::MatrixM, model_mat4);
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_Skybox
	//
	GLRenderCMD_Skybox::~GLRenderCMD_Skybox()
	{

	}

	GLRenderCMD_Skybox::GLRenderCMD_Skybox(Vertex* vertex, Transform* transform, Material* material)
		: RenderCommand(material->getShader())
		, mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	void GLRenderCMD_Skybox::run(BaseGraphics* graphics, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		graphics->setMat4(shader, ShaderParam::MatrixM, model_mat4);

		mMaterial->submit(graphics, shader);
		graphics->draw(mVertex);
	}


	//-------------------------------------------------------
	//
	//	RenderCMD_HDRToCube
	//
	GLRenderCMD_HDRToCube::GLRenderCMD_HDRToCube(Shader* shader, Vertex* vertex, Texture2D* hdr, TextureCube* cube)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mHDR(hdr)
		, mCube(cube)
	{

	}

	GLRenderCMD_HDRToCube::~GLRenderCMD_HDRToCube()
	{

	}

	void GLRenderCMD_HDRToCube::run(BaseGraphics* graphics, Shader* shader)
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
		graphics->setTexture2D(shader, ShaderParam::TexColor, mHDR);

		for (uint32_t i = 0; i < 6; i++)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , mCube->getTextureID()
								 , 0);

			graphics->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);
			graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			graphics->draw(mVertex);
		}

		glBindTexture(GL_TEXTURE_CUBE_MAP, mCube->getTextureID());
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BASE_LEVEL, 0);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_LEVEL, 5);
		glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	//-------------------------------------------------------
	//
	//	GLRenderCMD_EnvMakeIrradiance
	//
	GLRenderCMD_EnvMakeIrradiance::GLRenderCMD_EnvMakeIrradiance(Shader* shader, Vertex* vertex, TextureCube* cube, TextureCube* irradiance)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mCubeMap(cube)
		, mIrradianceMap(irradiance)
	{

	}

	GLRenderCMD_EnvMakeIrradiance::~GLRenderCMD_EnvMakeIrradiance()
	{

	}

	void GLRenderCMD_EnvMakeIrradiance::run(BaseGraphics* graphics, Shader* shader)
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
		graphics->setTextureCube(shader, ShaderParam::TexCube, mCubeMap);

		for (uint32_t i = 0; i < 6; i++)
		{
			//mFrameBuffer->attachCube(mIrradianceMap, 0, i);
			glFramebufferTexture2D(GL_FRAMEBUFFER
								 , GL_COLOR_ATTACHMENT0
								 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
								 , mIrradianceMap->getTextureID()
								 , 0);

			graphics->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);
			//shader->setViewMatrix(captureViews[i]);

			graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			graphics->draw(mVertex);
		}
	}

	//-------------------------------------------------------
	//
	//	GLRenderCMD_EnvMakePrefilter
	//
	GLRenderCMD_EnvMakePrefilter::GLRenderCMD_EnvMakePrefilter(Shader* shader
		, Vertex* vertex
		, TextureCube* cube
		, TextureCube* prefitler
		, uint32_t mipMaxLevel
		, uint32_t mipWidth
		, uint32_t mipHeight
		, float resolution)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mCubeMap(cube)
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

	void GLRenderCMD_EnvMakePrefilter::run(BaseGraphics* graphics, Shader* shader)
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
		graphics->setTextureCube(shader, ShaderParam::TexCube, mCubeMap);

		for (uint32_t mip = 0; mip < mMipMaxLevel; ++mip)
		{
			uint32_t mip_width = static_cast<uint32_t>(mMipWidth * std::pow(0.5, mip));
			uint32_t mip_height = static_cast<uint32_t>(mMipHeight * std::pow(0.5, mip));
			graphics->setViewport(ViewportInfo(0, 0, mip_width, mip_height));

			float roughness = (float)mip / (float)(mMipMaxLevel - 1);
			graphics->setFloat1(shader, "roughness", &roughness);
			graphics->setFloat1(shader, "resolution", &mResolution);

			for (uint32_t i = 0; i < 6; ++i)
			{
				//mFrameBuffer->attachCube(mPrefilterMap, 0, i, mip);
				glFramebufferTexture2D(GL_FRAMEBUFFER
									 , GL_COLOR_ATTACHMENT0
									 , GL_TEXTURE_CUBE_MAP_POSITIVE_X + i
									 , mPrefilterMap->getTextureID()
									 , mip);

				graphics->setMat4(shader, ShaderParam::MatrixV, captureViews[i]);

				graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
				graphics->draw(mVertex);
			}
		}
	}
}


