#pragma once

#include "../Head/Context.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class MeshData;
	class Vertex;

	enum class CMDLife
	{
		Once,
		Countor,
		Continue,
		Death
	};


	/// <summary>
	/// 
	/// </summary>
	class RenderCommand : public TinyObject
	{
		RenderCommand();
		TINY_RTTI_H(RenderCommand)

	public:
		virtual ~RenderCommand();

		const CMDLife& getLife() const { return mLife; }
		Vertex* getVertex() { return mVertex; }
		virtual void submit() = 0;

	private:
		CMDLife mLife = CMDLife::Continue;
		Vertex* mVertex = nullptr;
	};


	class RenderLine
	{
	public:


	private:

	};
}

