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
#include "Core/Manager/LightingManager.h"

namespace tezcat::Tiny
{
	RenderCommand::RenderCommand()
	{

	}

	RenderCommand::~RenderCommand()
	{

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
		: mShader(shader)
	{

	}

	RenderCMD_DeleteShader::~RenderCMD_DeleteShader()
	{
		mShader = nullptr;
	}

	void RenderCMD_DeleteShader::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->deleteShader(mShader);
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
		Graphics::getInstance()->deleteShader(mShader);
		mShader->rebuild();
		Graphics::getInstance()->createShader(mShader);
	}
#pragma endregion


	RenderCMD_DrawMesh::RenderCMD_DrawMesh(Vertex* vertex, Transform* transform, Material* material)
		: mVertex(vertex)
		, mTransform(transform)
		, mMaterial(material)
	{

	}

	RenderCMD_DrawMesh::~RenderCMD_DrawMesh()
	{
		mVertex = nullptr;
		mTransform = nullptr;
		mMaterial = nullptr;
	}

	void RenderCMD_DrawMesh::execute(PipelinePass* pass, Shader* shader)
	{
		auto& model_mat4 = mTransform->getModelMatrix();
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, model_mat4);

		glm::mat3 normal_matrix(model_mat4);
		Graphics::getInstance()->setMat3(shader, ShaderParam::MatrixN
			, glm::inverseTranspose(normal_matrix));

		mMaterial->submit(shader);
		Graphics::getInstance()->draw(mVertex);
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





	RenderCMD_DrawShadow::RenderCMD_DrawShadow(Vertex* vertex, Transform* transform)
		: mVertex(vertex)
		, mTransform(transform)
	{

	}

	RenderCMD_DrawShadow::~RenderCMD_DrawShadow()
	{
		mVertex = nullptr;
		mTransform = nullptr;
	}

	void RenderCMD_DrawShadow::execute(PipelinePass* pass, Shader* shader)
	{
		Graphics::getInstance()->setMat4(shader, ShaderParam::MatrixM, mTransform->getModelMatrix());
		Graphics::getInstance()->draw(mVertex);
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
		Graphics::getInstance()->setTextureCube(shader, ShaderParam::TexSkybox, mSkybox);
		Graphics::getInstance()->setFloat1(shader, "myLod", mLod);
		Graphics::getInstance()->setBool(shader, "myIsHDR", mIsHdr);
		Graphics::getInstance()->setFloat1(shader, "myExposure", mExposure);
		Graphics::getInstance()->draw(mVertex);
	}

	RenderCMD_MakeHDR2Cube::RenderCMD_MakeHDR2Cube(Vertex* vertex, Texture2D* texHDR, TextureCube* skybox)
		: mVertex(vertex)
		, mTexHDR(texHDR)
		, mSkybox(skybox)
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

		Graphics::getInstance()->makeHDR2Cube(shader, pass->getObserver(), mVertex, mTexHDR, mSkybox);
	}

	RenderCMD_MakeEnvIrradiance::RenderCMD_MakeEnvIrradiance(Vertex* vertex, TextureCube* skybox, TextureCube* irradiance)
		: mVertex(vertex)
		, mSkybox(skybox)
		, mIrradiance(irradiance)
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
		Graphics::getInstance()->makeEnvIrradiance(shader, pass->getObserver(), mVertex, mSkybox, mIrradiance);
	}

	RenderCMD_MakeEnvPrefilter::RenderCMD_MakeEnvPrefilter(Vertex* vertex
		, TextureCube* skybox
		, TextureCube* prefitler
		, uint32 mipMaxLevel
		, uint32 mipWidth
		, uint32 mipHeight
		, int32 resolution)
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

}
