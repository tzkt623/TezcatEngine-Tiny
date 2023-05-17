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
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
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
	}

	//-------------------------------------------------------
	//
	//	RenderCMD_EnvMap
	//
	RenderCMD_EnvMap::RenderCMD_EnvMap(Vertex* vertex, TextureCube* cube, TextureCube* env, Shader* shader, FrameBuffer* frameBuffer)
		: RenderCommand(shader)
		, mVertex(vertex)
		, mCubeMap(cube)
		, mEnvMap(env)
		, mFrameBuffer(frameBuffer)
	{

	}

	RenderCMD_EnvMap::~RenderCMD_EnvMap()
	{

	}

	void RenderCMD_EnvMap::draw(BaseGraphics* graphics, Shader* shader)
	{
		glm::mat4 captureViews[] =
		{
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
		};

		shader->resetLocalState();
		shader->setTextureCube(ShaderParam::TexCube, mCubeMap);

		for (uint32_t i = 0; i < 6; i++)
		{
			mFrameBuffer->attachCube(mEnvMap, 0, i);
			shader->setViewMatrix(captureViews[i]);

			graphics->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			graphics->draw(mVertex);
		}
	}
}
