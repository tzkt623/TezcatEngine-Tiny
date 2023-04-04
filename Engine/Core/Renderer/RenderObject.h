#pragma once
#include "../Component/Component.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API RenderObjectType
	{
		MeshRenderer,
		Texture,
		RenderTexture,
		Skybox,
		Light
	};

	enum class TINY_API RenderViewerType
	{
		Camera,
		Light,
	};

	class Shader;
	class Material;

	/// <summary>
	/// 可渲染的目标
	/// </summary>
	class TINY_API IRenderObject
	{
	public:
		virtual ~IRenderObject() = default;
		virtual RenderObjectType getRenderObjectType() = 0;
		virtual Material* getMaterial() const = 0;

		/// <summary>
		/// 把自己发送到对应的管线中
		/// </summary>
		virtual void sendToRenderPass() = 0;
		/// <summary>
		/// 发送数据到shader
		/// </summary>
		virtual void submit(Shader* shader) = 0;

		/// <summary>
		/// 绘制方式
		/// 索引or顶点
		/// </summary>
		virtual DrawModeWrapper& getDrawMode() = 0;

		/// <summary>
		/// 顶点数量
		/// </summary>
		virtual int getVertexCount() const { return 0; }

		/// <summary>
		/// 索引数量
		/// </summary>
		virtual int getIndexCount() const { return 0; }
	};

	// 	class TINY_API RenderObejct : public ComponentT<RenderObejct>, public IRenderObejct
	// 	{
	// 	public:
	// 		virtual ~RenderObejct() = default;
	// 	};

	class TINY_API IRenderViewer
	{
	public:
		IRenderViewer()
			: m_CullMask(0)
		{

		}

		virtual ~IRenderViewer() = default;
		virtual void submit(Shader* shader) = 0;
		virtual RenderViewerType getRenderViewerType() = 0;
		virtual bool cullGameObject(GameObject* gameObject) = 0;

		bool cullLayerMask(uint32_t layerMask) { return (m_CullMask & layerMask) == layerMask; }

		void setCullLayerMask(uint32_t mask)
		{
			m_CullMask = mask;
			this->refreshCullMask();
		}

		void addCullLayerMask(uint32_t mask)
		{
			m_CullMask |= mask;
			this->refreshCullMask();
		}

		void removeCullLayerMask(uint32_t mask)
		{
			m_CullMask &= ~mask;
			this->refreshCullMask();
		}

		const std::vector<uint32_t>& getCullLayerList() const { return m_CullLayerList; }

	private:
		void refreshCullMask()
		{
			m_CullLayerList.clear();
			for (int i = 0; i < 32; i++)
			{
				if (m_CullMask & (1u << i))
				{
					m_CullLayerList.emplace_back(1u << i);
				}
			}
		}


	private:
		uint32_t m_CullMask;
		std::vector<uint32_t> m_CullLayerList;
	};
}
