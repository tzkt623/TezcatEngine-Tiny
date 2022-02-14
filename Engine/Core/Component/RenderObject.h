#pragma once
#include "../Component/Component.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class Scene;
	enum class TINY_API RenderObjectType
	{
		Mesh,
		Light,
		Texture,
		RenderTexture
	};

	class TINY_API IRenderObejct
	{
	public:
		virtual ~IRenderObejct() = 0;
		virtual void sendDataToGPU() = 0;
		virtual void submit(Shader* shader) = 0;
		virtual RenderObjectType getRenderObjectType() = 0;
	};

	/// <summary>
	/// ��Ⱦ����
	/// ָ��Ҫ����ִ����Ⱦ����Ķ���
	/// </summary>
	template<typename T>
	class TINY_API RenderObject : public ComponentT<T>, public IRenderObejct
	{
	public:
		RenderObject() {}
		virtual ~RenderObject() {}
	};
}