#pragma once

namespace tezcat::Tiny::Core
{
	class TINY_API BaseObject
	{
	public:
		BaseObject() = default;
		virtual ~BaseObject() = default;

		//关闭自己之后记得delete
		virtual void close() = 0;
	};
}
