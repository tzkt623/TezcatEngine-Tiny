#pragma once
#include "RenderConfig.h"

#include "../Component/Component.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"
#include "../Head/GLMHead.h"

namespace tezcat::Tiny::Core
{
	enum class TINY_API RenderObjectType
	{
		MeshRenderer,
		Texture,
		RenderTexture,
		Skybox,
		Light,
		Camera,
		ShadowMap
	};

	class Shader;
	class Material;
	class Pipeline;
	class FrameBuffer;

	/// <summary>
	/// 可渲染的目标
	/// </summary>
	class TINY_API IRenderObject
	{
	public:
		IRenderObject() = default;

		/*
		 * Author:	HCL
		 * Info:	2023|4|9
		 *
		 */
		virtual ~IRenderObject() = default;

		/*
		 * Info:		HCL|2023|4|8
		 * Access:
		 * Returns:
		 * Qualifier:
		 * Parameter:
		 * Comment:
		 */
		virtual RenderObjectType getRenderObjectType() = 0;

		/// <summary>
		/// 把自己发送到对应的管线中
		/// </summary>
		virtual void sendToRenderPass(const RenderPassType& passType) {}

		/*
		* Info:		HCL|2023|4|8
		* Access:		virtual public
		* Returns:		void
		* Qualifier:
		* Comment: 开始渲染
		*/
		virtual void beginRender() {}

		/*
		* Info:		HCL|2023|4|8
		* Access:		virtual public
		* Returns:		void
		* Qualifier:
		* Parameter:	Shader * shader
		* Comment: 向shader提交数据
		*/
		virtual void submit(Shader* shader) = 0;

		/*
		* Info:		HCL|2023|4|8
		* Access:		virtual public
		* Returns:		void
		* Qualifier:
		* Comment: 结束渲染
		*/
		virtual void endRender() {}


	};

	class TINY_API IRenderMesh : public IRenderObject
	{
	public:
		IRenderMesh() = default;
		virtual ~IRenderMesh() = default;


		RenderObjectType getRenderObjectType() override { return RenderObjectType::MeshRenderer; }


		/*
		* @Author:	HCL
		* @Info:	2023|4|8
		* 材质
		*/
		virtual Material* getMaterial() const { return nullptr; }

		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 绘制方式 索引or顶点
		*/
		virtual DrawModeWrapper& getDrawMode() = 0;

		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 顶点数量
		*/
		virtual int getVertexCount() const { return 0; }


		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 索引数量
		*/
		virtual int getIndexCount() const { return 0; }

		/*
		* @Author: HCL
		* @Info: 2023|4|12
		* 提交模型矩阵数据
		*/
		virtual void submitModelMatrix(Shader* shader) {}
	};

	class TINY_API IRenderObserver : public IRenderObject
	{
	public:
		enum class Type
		{
			Ortho,
			Perspective
		};

	public:
		IRenderObserver();
		virtual ~IRenderObserver() = default;


		/*
		* @Author: HCL
		* @Info: 2023|4|12
		* 提交观察矩阵数据
		*/
		virtual void submitViewMatrix(Shader* shader) {}


		virtual bool culling(GameObject* gameObject) { return true; }

		bool cullLayerMask(uint32_t index)
		{
			auto mask = 1 << index;
			return (mCullMask & mask) == mask;
		}

		void setCullLayer(uint32_t index)
		{
			mCullMask = 1 << index;
			mCullLayerList.clear();
			mCullLayerList.push_back(index);
		}

		void addCullLayer(uint32_t index)
		{
			mCullMask |= (1 << index);
			mCullLayerList.push_back(index);
		}

		void removeCullLayer(uint32_t index)
		{
			mCullMask &= ~(1 << index);
			mCullLayerList.erase(std::find(mCullLayerList.begin(), mCullLayerList.end(), index));
		}

		const std::vector<uint32_t>& getCullLayerList() const { return mCullLayerList; }

		FrameBuffer* getFrameBuffer() const { return mFrameBuffer; }
		void setFrameBuffer(FrameBuffer* val) { mFrameBuffer = val; }
		void setViewRect(int x, int y, int width, int height);

		ViewportInfo& getViewportInfo() { return mViewInfo; }
		glm::mat4& getProjectionMatrix() { return mProjectionMatrix; }

		virtual glm::mat4& getViewMatrix() = 0;
		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);
		void updateObserverMatrix();

	protected:
		FrameBuffer* mFrameBuffer;
		ViewportInfo mViewInfo;

	protected:
		Type mType;
		float mNearFace;
		float mFarFace;
		float mFOV;

		glm::mat4 mProjectionMatrix;
		bool mPMatDirty;

	private:
		uint32_t mCullMask;
		std::vector<uint32_t> mCullLayerList;
	};
}
