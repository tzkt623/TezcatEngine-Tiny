#pragma once

#include "../Head/RenderConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class MeshData;
	class Vertex;
	class RenderAgent;
	class BaseGraphics;
	class Shader;
	class Transform;
	class Material;
	class Texture2D;
	class TextureCube;
	class TextureRender2D;
	class FrameBuffer;

	enum class CMDLife
	{
		Once,
		Countor,
		Continue,
		Death
	};



	/*
	* RenderCommand
	* @brief 负责生成当前需要的绘制命令,并且将自己发送到对应的Pass中
	* @brief 渲染命令每一帧都会生成,这样就与渲染对象进行了解耦
	*        可以支持一个对象在多个不同pass中渲染的需求
	*        只需要生成对应pass需要的绘制命令并发送即可
	*        也可以支持在任意位置直接生成渲染命令进行发送
	* @brief 渲染命令间接拥有它使用的Shader的数据,所以它自己就能找到自己应该存在的Pass
	*/
	class RenderCommand
	{
	public:
		union
		{
			uint64_t mOrderID;

			struct
			{
				uint64_t shaderID : 16;
				uint64_t priority : 32;
				uint64_t userID : 16;
			} mBase;

			struct
			{
				uint64_t shaderID : 16;
				uint64_t distanceFromCamera : 32;
				uint64_t userID : 16;
			} mTransparent;
		};
	protected:
		RenderCommand(Shader* shader);

	public:
		virtual ~RenderCommand();
		virtual void draw(BaseGraphics* graphics, Shader* shader) = 0;

		Shader* getShader() { return mShader; }

	protected:
		Shader* mShader;
	};

	class RenderCMD_Vertex : public RenderCommand
	{
	public:
		RenderCMD_Vertex(Vertex* vertex, Shader* shader);
		virtual ~RenderCMD_Vertex();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
	};


	class RenderCMD_Shadow : public RenderCommand
	{
	public:
		RenderCMD_Shadow(Vertex* vertex, Transform* transform);
		virtual ~RenderCMD_Shadow();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Transform* mTransform;
		Vertex* mVertex;
	};

	class RenderCMD_Skybox : public RenderCommand
	{
	public:
		RenderCMD_Skybox(Vertex* vertex, Transform* transform, Material* material);
		virtual ~RenderCMD_Skybox();

		void draw(BaseGraphics* graphics, Shader* shader) override;
	private:
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};

	class RenderCMD_Mesh : public RenderCommand
	{
	public:
		RenderCMD_Mesh(Vertex* vertex, Transform* transform, Material* material);
		virtual ~RenderCMD_Mesh();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};

	class RenderCMD_HDRToCube : public RenderCommand
	{
	public:
		RenderCMD_HDRToCube(Vertex* vertex, Texture2D* hdr, TextureCube* cube, Shader* shader, FrameBuffer* frameBuffer);
		virtual ~RenderCMD_HDRToCube();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		Texture2D* mHDR;
		TextureCube* mCube;
		FrameBuffer* mFrameBuffer;
	};

	class RenderCMD_EnvMakeIrradiance : public RenderCommand
	{
	public:
		RenderCMD_EnvMakeIrradiance(Vertex* vertex, TextureCube* cube, TextureCube* irradiance, Shader* shader, FrameBuffer* frameBuffer);
		virtual ~RenderCMD_EnvMakeIrradiance();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mCubeMap;
		TextureCube* mIrradianceMap;
		FrameBuffer* mFrameBuffer;
	};

	class RenderCMD_EnvMakePrefilter : public RenderCommand
	{
	public:
		RenderCMD_EnvMakePrefilter(Vertex* vertex
			, TextureCube* cube
			, TextureCube* prefitler
			, Shader* shader
			, FrameBuffer* frameBuffer
			, uint32_t mipMaxLevel
			, uint32_t mipWidth
			, uint32_t mipHeight
			, float resolution);
		virtual ~RenderCMD_EnvMakePrefilter();

		void draw(BaseGraphics* graphics, Shader* shader) override;

	private:
		Vertex* mVertex;
		TextureCube* mCubeMap;
		TextureCube* mPrefilterMap;
		FrameBuffer* mFrameBuffer;
		uint32_t mMipMaxLevel;
		uint32_t mMipWidth;
		uint32_t mMipHeight;
		float mResolution;
	};

	class RenderCMD_Bath2D : public RenderCommand
	{

	};
}

