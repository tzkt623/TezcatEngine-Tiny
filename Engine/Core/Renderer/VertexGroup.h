#pragma once

#include "../Head/CppHead.h"
#include "../Tool/DelegateCreator.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class VertexBuffer;

	class TINY_API VertexGroup
	{
	public:
		VertexGroup();
		virtual ~VertexGroup();

	public:
		std::string getName() const { return mName; }
		void setName(const std::string& val) { mName = val; }
		int getVertexCount() const { return mVertexCount; }
		int getIndexCount() const { return mIndexCount; }
		unsigned int getUID() const { return mUID; }

	public:
		virtual void init(MeshData* mesh) = 0;
		virtual void bind() = 0;
		virtual void unbind() = 0;

	protected:
		std::string mName;
		unsigned int mUID;

		int mVertexCount;
		int mIndexCount;

		VertexBuffer* mVertexBuffer;
	};

	class VertexGroupCreatorImp
	{
	public:
		virtual ~VertexGroupCreatorImp() = default;
		virtual VertexGroup* create(MeshData* meshData) = 0;
	};

	using VertexGroupCreator = DelegateCreator<VertexGroupCreatorImp, VertexGroup>;
}
