#pragma once
#include "../Component/Component.h"
#include "../Head/RenderConfig.h"
#include "../Head/GLMHead.h"

#include "Vertex.h"
#include "RenderAgent.h"
#include "../Data/Material.h"


namespace tezcat::Tiny
{
	enum class TINY_API RenderObjectType
	{
		Observer,
		MeshRenderer,
		Light
	};

	class Shader;
	class FrameBuffer;
	class RenderQueue;
	class BaseGraphics;



	/*
	* IRenderObject
	*
	* @brief 可渲染对象
	*/
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


		/*
		* @Author: HCL
		* @Info: 2023|5|15
		*
		*/
		virtual void sendToQueue(BaseGraphics* graphics, const RenderPhase& phase, RenderQueue* queue) {}

		/*
		* Info:		HCL|2023|4|8
		* Access:		virtual public
		* Returns:		void
		* Qualifier:
		* Parameter:	Shader * shader
		* Comment: 向shader提交数据
		*/
		virtual void submit(BaseGraphics* graphics, Shader* shader) = 0;
	};

	/*
	* IRenderMesh
	*
	* @brief 可渲染mesh
	*/
	class TINY_API IRenderMesh : public IRenderObject
	{
	public:
		IRenderMesh();
		virtual ~IRenderMesh();


		RenderObjectType getRenderObjectType() final { return RenderObjectType::MeshRenderer; }

		/*
		* @Author:	HCL
		* @Info:	2023|4|8
		* 材质
		*/
		Material* getMaterial() const { return mMaterial; }
		void setMaterial(Material* material);

		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 顶点数量
		*/
		int getVertexCount() const
		{
			return mVertex->getVertexCount();
		}


		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 索引数量
		*/
		int getIndexCount() const
		{
			return mVertex->getIndexCount();
		}

		void setMesh(const std::string& meshName);
		void setMesh(MeshData* meshData);

		void submit(BaseGraphics* graphics, Shader* shader) override;

		void setShadowReciever(bool value)
		{
			mIsShadowReciever = true;
		}

		bool isShadowReciever() const { return mIsShadowReciever; }

	protected:
		bool mIsShadowReciever;
		RenderAgent* mRenderAgent;
		Material* mMaterial;
		Vertex* mVertex;
	};


	/*
	* IRenderObserver
	*
	* @brief 渲染观察者
	* @brief 观察者可以用它的视角来观察整个场景,并提供观察视角的数据
	* @brief 观察者可以指定它看到的图元被渲染到哪个帧缓存上
	* @brief 所以渲染目标必须在最外层被绑定,也就是整个目标在渲染时不能切换到其他目标上
	*        于是要对Queue内部的Pass和CMD等进行排序
	*
	* @brief 所以基于Observer的渲染结构如下
	*        foreach observers
	*          observer check layer to culling agent
	*            agent make CMD to current observer`s Pass
	*        foreach passes
	*          pass render
	*/
	class TINY_API IRenderObserver : public IRenderObject
	{
	public:
		enum class ViewType : uint8_t
		{
			Screen,
			Ortho,
			Perspective,	
		};

	public:
		IRenderObserver(RenderQueue* queue);
		virtual ~IRenderObserver();

		RenderObjectType getRenderObjectType() final { return RenderObjectType::Observer; }

		virtual void submitViewMatrix(BaseGraphics* graphics, Shader* shader) {}

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

		FrameBuffer* getFrameBuffer() { return mFrameBuffer; }
		void setFrameBuffer(FrameBuffer* buffer);

		void setViewRect(int x, int y, int width, int height);
		ViewportInfo& getViewportInfo() { return mViewInfo; }
		glm::mat4& getProjectionMatrix() { return mProjectionMatrix; }
		glm::mat4& getViewMatrix() { return mViewMatrix; }

		void setOrtho(float near, float far);
		void setPerspective(float fov, float near, float far);

		const RenderPhase& getPipelinePhase() const { return mRenderPhase; }
		void setPipelineType(RenderPhase type) { mRenderPhase = type; }

		void setClearOption(ClearOptionID option) { mClearOption = (ClearOption)option; }
		const ClearOption& getClearOption() const { return mClearOption; }

		void updateObserverMatrix();

		int getOrder() const { return mOrder; }
		void setOrder(int val) { mOrder = val; }

		float getFOV() { return mFOV; }
		void setFOV(float fov)
		{
			mFOV = fov;
			mDirty = true;
		}

		float getAspect() { return (float)mViewInfo.Width / (float)mViewInfo.Height; }

		void setNear(float near)
		{
			mNearFace = near;
			mDirty = true;
		}
		float getNear() { return mNearFace; }
		void setFar(float far)
		{
			mFarFace = far;
			mDirty = true;
		}
		float getFar() { return mFarFace; }

		RenderQueue* getRenderQueue() { return mRenderQueue; }
		template<class QueueType>
		QueueType* getRenderQueue() { return static_cast<QueueType*>(mRenderQueue); }

		ViewType getViewType() { return mViewType; }

	protected:
		FrameBuffer* mFrameBuffer;
		ViewportInfo mViewInfo;

	protected:
		int mOrder;
		RenderPhase mRenderPhase;
		ViewType mViewType;
		float mNearFace;
		float mFarFace;
		float mFOV;

		glm::mat4 mProjectionMatrix;
		glm::mat4 mViewMatrix;
		bool mDirty;

	protected:
		uint32_t mCullMask;
		std::vector<uint32_t> mCullLayerList;
		ClearOption mClearOption;
		RenderQueue* mRenderQueue;
	};
}
