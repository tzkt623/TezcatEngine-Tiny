#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class MeshData;
	class VertexBuffer;

	/// <summary>
	/// 对应gl的VAO
	/// </summary>
	class TINY_API Vertex
	{
	public:
		Vertex();
		virtual ~Vertex();

	public:
		std::string getName() const { return mName; }
		void setName(const std::string& val) { mName = val; }
		int getVertexCount() const { return mVertexCount; }
		int getIndexCount() const { return mIndexCount; }
		unsigned int getUID() const { return mUID; }

		void addChild(Vertex* vertex);

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

		std::vector<Vertex*>* mChildren;
	};

	class TINY_API VertexGroupCreatorImp
	{
	public:
		virtual ~VertexGroupCreatorImp() = default;
		virtual Vertex* create(MeshData* meshData) = 0;
	};

	//using VertexGroupCreator = DelegateCreator<VertexGroupCreatorImp, Vertex>;
}
