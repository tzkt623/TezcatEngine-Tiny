#pragma once

#include "../Head/TinyCpp.h"

namespace tezcat::Tiny
{
	class BaseGraphics;
	class Shader;
	class Vertex;
	class Material;
	class Transform;


	enum class CMDLife
	{
		Once,
		Countor,
		Continue,
		Death
	};

	enum class CommandID : uint32
	{
		None = 0,
		DrawMesh,
	};


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
		RenderCommand();
		RenderCommand(Vertex* vertex, Transform* transform, Material* material);
		



	public:
		virtual ~RenderCommand();
		virtual void run(Shader* shader) = 0;

	public:
		uint32 mCommandID;
		Vertex* mVertex;
		Transform* mTransform;
		Material* mMaterial;
	};
}

