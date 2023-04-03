#pragma once

#include "Core/Renderer/VertexBuffer.h"
#include "Core/Renderer/VertexGroup.h"
#include "Core/Head/GLMHead.h"

using namespace tezcat::Tiny::Core;
namespace tezcat::Tiny::GL
{
	class TINY_API GLVertexBuffer : public VertexBuffer
	{
	public:
		GLVertexBuffer(MeshData* meshData);
		virtual ~GLVertexBuffer();

	protected:
		void init(MeshData* meshData) override;

	private:
		uint32_t* mVBOArray;
		int mVBOSize;
	};


	class TINY_API GLVertexGroup : public VertexGroup
	{
	public:
		GLVertexGroup(MeshData* meshData);
		virtual ~GLVertexGroup();

		void init(MeshData* mesh) override;
		void bind() override;
		void unbind() override;
	};


	class TINY_API GLVertexBufferCreator : public VertexBufferCreatorImp
	{
	public:
		VertexBuffer* create(MeshData* meshData) override;
	};

	class TINY_API GLVertexGroupCreator : public VertexGroupCreatorImp
	{
	public:
		VertexGroup* create(MeshData* meshData) override;
	};
}
