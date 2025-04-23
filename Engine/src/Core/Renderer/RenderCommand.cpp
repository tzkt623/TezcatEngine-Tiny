/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/BaseGraphics.h"
#include "Core/Renderer/FrameBuffer.h"
#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/Texture.h"
#include "Core/Renderer/Vertex.h"
#include "Core/Renderer/Material.h"
#include "Core/Renderer/Pipeline.h"
#include "Core/Renderer/RenderObserver.h"

#include "Core/Shader/Shader.h"

#include "Core/Component/Transform.h"
#include "Core/Component/GameObject.h"

#include "Core/Manager/LightingManager.h"
#include "Core/Manager/FrameBufferManager.h"
#include "Core/Manager/GameObjectManager.h"

#include "Core/Event/EngineEvent.h"

#include "Core/Debug/Debug.h"

namespace tezcat::Tiny
{
	RenderCommand::RenderCommand()
	{

	}

	RenderCommand::~RenderCommand()
	{

	}

	RenderCMD_Lambda::RenderCMD_Lambda(std::function<void(PipelinePass*, Shader*)> function)
		: mFuncion(function)
	{

	}

	void RenderCMD_Lambda::execute(PipelinePass* pass, Shader* shader)
	{
		TINY_PIPELINE_INFO_PUSH("LambdaDraw");
		mFuncion(pass, shader);
		TINY_PIPELINE_INFO_POP();
	}

#pragma region Shader
	RenderCMD_CreateShader::RenderCMD_CreateShader(Shader* shader)
		: mShader(shader)
	{

	}

	RenderCMD_CreateShader::~RenderCMD_CreateShader()
	{
		mShader = nullptr;
	}

	void RenderCMD_CreateShader::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createShader(mShader);
	}

	RenderCMD_DeleteShader::RenderCMD_DeleteShader(Shader* shader)
		: mID(shader->getProgramID())
	{

	}

	RenderCMD_DeleteShader::~RenderCMD_DeleteShader()
	{
		//mShader = nullptr;
	}

	void RenderCMD_DeleteShader::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteShader(mID);
	}

	RenderCMD_RebuildShader::RenderCMD_RebuildShader(Shader* shader)
		: mShader(shader)
	{

	}

	RenderCMD_RebuildShader::~RenderCMD_RebuildShader()
	{
		mShader = nullptr;
	}

	void RenderCMD_RebuildShader::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteShader(mShader->getProgramID());
		mShader->rebuild();
		Graphics::getInstance()->createShader(mShader);
	}

#pragma endregion
	RenderCMD_DrawID::RenderCMD_DrawID(Vertex* vertex, Transform* transform)
		: mVertex(vertex)
		, mTransform(transform)
	{

	}

	RenderCMD_DrawID::~RenderCMD_DrawID()
	{
		mVertex = nullptr;
		mTransform = nullptr;
	}

	void RenderCMD_DrawID::execute(PipelinePass* pass, Shader* shader)
	{
		TINY_PIPELINE_INFO_PUSH(mTransform->getGameObject()->getName());

		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, mTransform->getModelMatrix());
		Graphics::getInstance()->setUInt1(shader, "myObjectID", mTransform->getGameObject()->getUID());
		Graphics::getInstance()->draw(mVertex);

		TINY_PIPELINE_INFO_POP();
	}

	RenderCMD_DrawMeshWithMaterial::RenderCMD_DrawMeshWithMaterial(Vertex* vertex, Transform* transform, Material* material)
		: mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	RenderCMD_DrawMeshWithMaterial::~RenderCMD_DrawMeshWithMaterial()
	{
		mVertex = nullptr;
		mTransform = nullptr;
		mMaterial = nullptr;
	}

	void RenderCMD_DrawMeshWithMaterial::execute(PipelinePass* pass, Shader* shader)
	{
		TINY_PIPELINE_INFO_PUSH(mTransform->getGameObject()->getName());

		auto& model_mat4 = mTransform->getModelMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, model_mat4);

		glm::mat3 normal_matrix(model_mat4);
		Graphics::getInstance()->setMat3(shader, ShaderParam::MatrixN
			, glm::inverseTranspose(normal_matrix));

		mMaterial->submit(shader);
		Graphics::getInstance()->draw(mVertex);

		TINY_PIPELINE_INFO_POP();
	}

	RenderCMD_CreateVertex::RenderCMD_CreateVertex(Vertex* vertex)
		: mVertex(vertex)
	{

	}

	RenderCMD_CreateVertex::~RenderCMD_CreateVertex()
	{
		mVertex = nullptr;
	}

	void RenderCMD_CreateVertex::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createVertex(mVertex);
	}

	RenderCMD_CreateFrameBuffer::RenderCMD_CreateFrameBuffer(FrameBuffer* buffer)
		: mBuffer(buffer)
	{

	}

	RenderCMD_CreateFrameBuffer::~RenderCMD_CreateFrameBuffer()
	{
		mBuffer = nullptr;
	}

	void RenderCMD_CreateFrameBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createBuffer(mBuffer);
	}

	RenderCMD_CreateTexture2D::RenderCMD_CreateTexture2D(Texture2D* tex)
		: mTex(tex)
	{

	}

	RenderCMD_CreateTexture2D::~RenderCMD_CreateTexture2D()
	{
		mTex = nullptr;
	}

	void RenderCMD_CreateTexture2D::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createTexture(mTex);
	}

	RenderCMD_CreateTextureRender2D::RenderCMD_CreateTextureRender2D(TextureRender2D* tex)
		: mTex(tex)
	{

	}

	RenderCMD_CreateTextureRender2D::~RenderCMD_CreateTextureRender2D()
	{
		mTex = nullptr;

	}

	void RenderCMD_CreateTextureRender2D::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createTexture(mTex);
	}

	RenderCMD_CreateTextureCube::RenderCMD_CreateTextureCube(TextureCube* tex)
		: mTex(tex)
	{

	}

	RenderCMD_CreateTextureCube::~RenderCMD_CreateTextureCube()
	{
		mTex = nullptr;
	}

	void RenderCMD_CreateTextureCube::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createTexture(mTex);
	}





	RenderCMD_DrawMeshWithOutMaterial::RenderCMD_DrawMeshWithOutMaterial(Vertex* vertex, Transform* transform)
		: mVertex(vertex)
		, mTransform(transform)
	{

	}

	RenderCMD_DrawMeshWithOutMaterial::~RenderCMD_DrawMeshWithOutMaterial()
	{
		mVertex = nullptr;
		mTransform = nullptr;
	}

	void RenderCMD_DrawMeshWithOutMaterial::execute(PipelinePass* pass, Shader* shader)
	{
		TINY_PIPELINE_INFO_PUSH(mTransform->getGameObject()->getName());

		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, mTransform->getModelMatrix());
		Graphics::getInstance()->draw(mVertex);

		TINY_PIPELINE_INFO_POP();
	}

	RenderCMD_DrawSkybox::RenderCMD_DrawSkybox(Vertex* vertex
		, TextureCube* skybox
		, float lod
		, bool isHdr
		, float exposure)
		: mVertex(vertex)
		, mSkybox(skybox)
		, mLod(lod)
		, mIsHdr(isHdr)
		, mExposure(exposure)
	{

	}

	RenderCMD_DrawSkybox::~RenderCMD_DrawSkybox()
	{

	}

	void RenderCMD_DrawSkybox::execute(PipelinePass* pass, Shader* shader)
	{
		TINY_PIPELINE_INFO_PUSH("SkyBox");

		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, mSkybox);
		Graphics::getInstance()->setFloat1(shader, "myLod", mLod);
		Graphics::getInstance()->setBool(shader, "myIsHDR", mIsHdr);
		Graphics::getInstance()->setFloat1(shader, "myExposure", mExposure);
		Graphics::getInstance()->draw(mVertex);

		TINY_PIPELINE_INFO_POP();
	}

	RenderCMD_MakeHDR2Cube::RenderCMD_MakeHDR2Cube(Vertex* vertex
		, Texture2D* texHDR
		, TextureCube* skybox
		, std::array<int32_t, 2> viewSize)
		: mVertex(vertex)
		, mTexHDR(texHDR)
		, mSkybox(skybox)
		, mViewSize(viewSize)
	{

	}

	RenderCMD_MakeHDR2Cube::~RenderCMD_MakeHDR2Cube()
	{

	}

	void RenderCMD_MakeHDR2Cube::execute(PipelinePass* pass, Shader* shader)
	{
		/*
		shader->resetLocalState();
		auto uinfo = shader->getUniformInfo("myTexHDR2D");
		Graphics::getInstance()->setTexture2D(shader, uinfo->shaderID, mTexHDR);

		UniformBuffer* buffer = pass->getObserver()->getUniformBuffer();
		auto array = LightingManager::getCubeMapTextureArray();

		for (uint32 i = 0; i < 6; i++)
		{
			buffer->updateWithConfig<UniformBufferBinding::SkyBox::ViewIndex>(&i);
			Graphics::getInstance()->setUniformBuffer(buffer);

			Graphics::getInstance()->setFrameBufferColorTexture2D(array[i], 0);
			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(mVertex);

			Graphics::getInstance()->setFrameBufferColorTextureCube(mSkybox, 0, i);
			Graphics::getInstance()->clear(ClearOption(ClearOption::CO_Color | ClearOption::CO_Depth));
			Graphics::getInstance()->draw(mVertex);
		}

		Graphics::getInstance()->createMipmapTexCube(mSkybox, 0, 5);
		*/

		Graphics::getInstance()->makeHDR2Cube(shader, pass->getObserver(), mVertex, mTexHDR, mSkybox, mViewSize);
	}

	RenderCMD_MakeEnvIrradiance::RenderCMD_MakeEnvIrradiance(Vertex* vertex, TextureCube* skybox, TextureCube* irradiance, std::array<int32_t, 2> viewSize)
		: mVertex(vertex)
		, mSkybox(skybox)
		, mIrradiance(irradiance)
		, mViewSize(viewSize)
	{

	}

	RenderCMD_MakeEnvIrradiance::~RenderCMD_MakeEnvIrradiance()
	{
		mVertex = nullptr;
		mSkybox = nullptr;
		mIrradiance = nullptr;
	}

	void RenderCMD_MakeEnvIrradiance::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->makeEnvIrradiance(shader, pass->getObserver(), mVertex, mSkybox, mIrradiance, mViewSize);
	}

	RenderCMD_MakeEnvPrefilter::RenderCMD_MakeEnvPrefilter(Vertex* vertex
		, TextureCube* skybox
		, TextureCube* prefitler
		, uint32_t mipMaxLevel
		, uint32_t mipWidth
		, uint32_t mipHeight
		, int32_t resolution)
		: mVertex(vertex)
		, mSkyboxMap(skybox)
		, mPrefilterMap(prefitler)
		, mMipMaxLevel(mipMaxLevel)
		, mMipWidth(mipWidth)
		, mMipHeight(mipHeight)
		, mResolution(resolution)
	{

	}

	RenderCMD_MakeEnvPrefilter::~RenderCMD_MakeEnvPrefilter()
	{
		mVertex = nullptr;
		mSkyboxMap = nullptr;
		mPrefilterMap = nullptr;
	}

	void RenderCMD_MakeEnvPrefilter::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->makeEnvPrefilter(shader
			, pass->getObserver()
			, mVertex
			, mSkyboxMap
			, mPrefilterMap
			, mMipMaxLevel
			, mMipWidth
			, mMipHeight
			, mResolution);
	}

	RenderCMD_DrawVertex::RenderCMD_DrawVertex(Vertex* vertex)
		: mVertex(vertex)
	{

	}

	RenderCMD_DrawVertex::~RenderCMD_DrawVertex()
	{
		mVertex = nullptr;
	}

	void RenderCMD_DrawVertex::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->draw(mVertex);
	}

	RenderCMD_DeleteTextureRender2D::RenderCMD_DeleteTextureRender2D(TextureRender2D* tex)
		: mID(tex->getTextureID())
	{

	}

	RenderCMD_DeleteTextureRender2D::~RenderCMD_DeleteTextureRender2D()
	{

	}

	void RenderCMD_DeleteTextureRender2D::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteRender2D(mID);
	}

	RenderCMD_DeleteTextureCube::RenderCMD_DeleteTextureCube(TextureCube* tex)
		: mID(tex->getTextureID())
	{

	}

	RenderCMD_DeleteTextureCube::~RenderCMD_DeleteTextureCube()
	{

	}

	void RenderCMD_DeleteTextureCube::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteTextureCube(mID);
	}

	RenderCMD_DeleteTexture2D::RenderCMD_DeleteTexture2D(Texture2D* tex)
		: mID(tex->getTextureID())
	{

	}

	RenderCMD_DeleteTexture2D::~RenderCMD_DeleteTexture2D()
	{

	}

	void RenderCMD_DeleteTexture2D::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteTexture2D(mID);
	}

	RenderCMD_CreateUniformBuffer::RenderCMD_CreateUniformBuffer(UniformBuffer* buffer, int32_t index)
		: mBuffer(buffer)
		, mIndex(index)
	{

	}

	RenderCMD_CreateUniformBuffer::~RenderCMD_CreateUniformBuffer()
	{

	}

	void RenderCMD_CreateUniformBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->createUniformBuffer(mBuffer, mIndex);
	}



	RenderCMD_ReadObjectID::RenderCMD_ReadObjectID(int x, int y, FrameBuffer* fb)
		: mX(x)
		, mY(y)
		, mFrameBuffer(fb)
	{

	}

	RenderCMD_ReadObjectID::~RenderCMD_ReadObjectID()
	{

	}

	void RenderCMD_ReadObjectID::execute(PipelinePass* pass, Shader* shader)
	{
		int32_t id;
		Graphics::getInstance()->bind(mFrameBuffer);
		Graphics::getInstance()->readObjectID(mX, mY, id);
		Graphics::getInstance()->unbind(mFrameBuffer);
		EngineEvent::getInstance()->dispatch({ EngineEventID::EE_SelectObject, GameObjectManager::getObject(id) });
		TINY_LOG_INFO(std::format("Pick ObjectID {}", id));
	}


	RenderCMD_ClearTexture2D::RenderCMD_ClearTexture2D(Texture2D* tex)
		: mTexture(tex)
	{

	}

	RenderCMD_ClearTexture2D::~RenderCMD_ClearTexture2D()
	{

	}

	void RenderCMD_ClearTexture2D::execute(PipelinePass* pass, Shader* shader)
	{
		auto frame_buffer = FrameBuffer::create();
		frame_buffer->addAttachment(mTexture);

		Graphics::getInstance()->createBuffer(frame_buffer);
		Graphics::getInstance()->bind(frame_buffer);
		Graphics::getInstance()->setClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		Graphics::getInstance()->clear(ClearOption::CO_Color);
		Graphics::getInstance()->unbind(frame_buffer);
	}

	RenderCMD_DeleteFrameBuffer::RenderCMD_DeleteFrameBuffer(FrameBuffer* frameBuffer)
		: mID(frameBuffer->getFrameBufferID())
	{

	}

	RenderCMD_DeleteFrameBuffer::~RenderCMD_DeleteFrameBuffer()
	{

	}

	void RenderCMD_DeleteFrameBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteFrameBuffer(mID);
	}

	RenderCMD_DeleteVertex::RenderCMD_DeleteVertex(Vertex* buffer)
		: mID(buffer->getVertexID())
	{

	}

	RenderCMD_DeleteVertex::~RenderCMD_DeleteVertex()
	{

	}

	void RenderCMD_DeleteVertex::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteVertex(mID);
	}

	RenderCMD_DeleteVertexBuffer::RenderCMD_DeleteVertexBuffer(VertexBuffer* buffer)
		: mID(buffer->getBufferID())
	{

	}

	RenderCMD_DeleteVertexBuffer::~RenderCMD_DeleteVertexBuffer()
	{

	}

	void RenderCMD_DeleteVertexBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteVertexBuffer(mID);
	}

	RenderCMD_DeleteIndexBuffer::RenderCMD_DeleteIndexBuffer(IndexBuffer* buffer)
		: mID(buffer->getBufferID())
	{

	}

	RenderCMD_DeleteIndexBuffer::~RenderCMD_DeleteIndexBuffer()
	{

	}

	void RenderCMD_DeleteIndexBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteIndexBuffer(mID);
	}

	RenderCMD_DeleteUniformBuffer::RenderCMD_DeleteUniformBuffer(UniformBuffer* buffer)
		: mID(buffer->getBufferID())
	{

	}

	RenderCMD_DeleteUniformBuffer::~RenderCMD_DeleteUniformBuffer()
	{

	}

	void RenderCMD_DeleteUniformBuffer::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteUniformBuffer(mID);
	}

}
