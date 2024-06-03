#pragma once

#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Shader;

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
		virtual void run(BaseGraphics* graphics, Shader* shader) = 0;

		Shader* getShader() { return mShader; }

	protected:
		Shader* mShader;
	};
}

