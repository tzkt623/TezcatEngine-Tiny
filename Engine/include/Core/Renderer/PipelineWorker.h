#pragma once
/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../Base/TinyObject.h"
#include "../Head/TinyCpp.h"
#include "../Tool/Tool.h"
#include "RenderObserver.h"

namespace tezcat::Tiny
{

	class ShadowCasterManager;
	class LightingManager;
	class LightManager;
	class RenderPass;
	class BaseRenderObserver;
	class BaseMeshRenderer;
	class RenderCommand;
	class Shader;
	class FrameBuffer;
	class Transform;
	class PipelineQueue;

	/*
	* Pipeline整体设计
	*
	* Pipeline:
	*   PipelineQueue: 每一个观察者对应一个Queue
	*     PipelinePass: 每一个Shader对应一个Pass
	*       RenderCommand
	*
	* 每一帧遍历所有queue执行里面的cmd进行渲染
	* Queue的排序
	*	按照相机的OrderID进行排序
	*	OrderID越小越先进行渲染
	*
	* 例如.
	*	前处理ObseverQueue1
	*	......
	*	前处理ObseverQueueN
	*
	*	玩家相机Queue1
	*	......
	*	玩家相机QueueN
	*
	*	后处理ObseverQueue1
	*	......
	*	后处理ObseverQueueN
	*/
	class TINY_API PipelineWorker : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(PipelineWorker, TinyObject)
	protected:
		PipelineWorker();

	public:
		virtual ~PipelineWorker() {}

		virtual void setObserver(BaseRenderObserver* observer);
		BaseRenderObserver* getObserver() const { return mRenderObserver.lock(); }

		virtual void addToPipeline() = 0;
		void removeFromPipeline();

		virtual void onEnterPipeline();
		virtual void onExitPipeline();

		bool isNeedRemoved() const { return mRemoveFlag && mIsInPipeline; }

	protected:
		bool mIsInPipeline;
		bool mRemoveFlag;
		TinyWeakRef<BaseRenderObserver> mRenderObserver;
	};

	class TINY_API PipelinePass : public PipelineWorker
	{
	public:
		enum class Mode : int8_t
		{
			//持续存在型
			Continued = 0,
			//只存在一次型
			Once
		};

		enum GlobalSubmit : uint32_t
		{
			GLOBAL_ALL = 0,
			NO_SHADOW = 1 << 0,
			NO_LIGHTING = 1 << 1,
			NO_ENVLIGHTING = 1 << 2,
			GLOBAL_NONE = std::numeric_limits<uint32_t>::max()
		};

		TINY_ABSTRACT_OBJECT_H(PipelinePass, PipelineWorker)

	protected:
		PipelinePass(uint32_t globalSubmitMask);

	public:
		virtual ~PipelinePass();

		void setObserver(BaseRenderObserver* observer) override;

		void render(BaseRenderObserver* observer);

		void addGlobalSubmit(std::function<void(Shader*)>& function)
		{
			mGlobalSubmitArray.emplace_back(function);
		}

		void resetGlobalFunction(uint32_t globalSubmitMask);

		//Mode
	public:
		/*
		* 持续模式
		*/
		void setContinuedMode() { mMode = Mode::Continued; }
		/*
		* 单次模式
		*/
		void setOnceMode() { mMode = Mode::Once; }
		const Mode getMode() const { return mMode; }
		/*
		* 重置单次模式
		*/
		void resetOnceMode() { mIsOnceModeExecuted = false; }
		std::string getMemoryInfo() override;

	public:
		/*
		* OrderID used by pipeline
		*/
		uint32_t getUserSortingID() const { return mSortingIDType2.user; }

		/*
		* User Set Custom OrderID
		* Range [0, 65535]
		*/
		void setUserSortingID(uint16_t value)
		{
			mSortingIDType2.user = value;
		}

		void setName(std::string name)
		{
			mName.assign(std::move(name));
		}

		/*
		* PipelineID: uint32_t(8-8-16)
		* 相机OrderID(-128-127)-队列OrderID(0-255)-PassOrderID(0-65535)
		*
		* 排序时:
		* 1.先排相机
		* 2.再排相机中的队列
		* 3.再排队列中的通道
		*/
		uint64_t getPipelineSortingID() const { return mPipelineSortingID; }

		/*
		* 通道的唯一ID
		*/
		uint32_t getUID() const;

		void addCommand(RenderCommand* cmd);

		template<class CMD, class... Args>
		void addCommand(Args&&... args)
		{
			mCommandArray.push_back(new CMD(TINY_FWD(args)...));
		}

		void setFrameBuffer(FrameBuffer* frameBuffer);

		/*
		* 将此pipelinepass加入pipeline
		*/
		void addToPipeline() override;

		Shader* getShader() { return mShader; }

	protected:
		void beginRender();
		void onRender();
		void endRender();

		void setShader(Shader* shader);

		virtual void onClose() override;

	protected:
		/*
		*
		*/
		union
		{
			uint64_t mPipelineSortingID;

			struct Type1
			{
				uint64_t renderType : 8;
				uint64_t observerOrder : 8;
				uint64_t frameBufferOrder : 16;
				uint64_t shader : 8;
				uint64_t user : 16;
			} mCombineOrderID;

			struct Type2
			{
				//从低位到高位

				uint64_t temp : 32;
				uint64_t user : 16;

				/*
				* ShaderQueue
				*
				* Prepare : 1000
				* Background : 2000
				* Opaque : 3000
				* Transparent : 4000
				* Post : 5000
				*/
				uint64_t shader : 16;
			} mSortingIDType2;
		};

	protected:
		std::string mName;
		Mode mMode;
		bool mIsOnceModeExecuted;
		bool mFrameBufferChanged;
		Shader* mShader;
		FrameBuffer* mFrameBuffer;
		std::vector<RenderCommand*> mCommandArray;
		std::vector<std::function<void(Shader*)>> mGlobalSubmitArray;
		PipelineQueue* mQueue;
	};

	/*
	* 观察者通道
	* 1.观察者主动遍历Layer,找到合适的Renderer并让其自动生成CMD并加入到Pass中
	* 2.在此模式下Pass和CMD都是自动生成
	*
	* 注意:此通道不会管理其中Observer的生命周期
	*/
	class TINY_API ObserverPipelinePass : public PipelinePass
	{
		friend class BaseRenderObserver;
		friend class PipelineManager;
		friend class PipelineQueue;
		TINY_OBJECT_H_VISIT(private, ObserverPipelinePass, PipelinePass)
	private:
		ObserverPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32_t globalFunction = GLOBAL_ALL);
	public:
		virtual ~ObserverPipelinePass();
	};

	/*
	* 代替通道
	* 1.此通道可以代替当前Mesh使用的Shader来渲染
	*	例如.渲染阴影.渲染ID
	* 2.此通道可以自定义渲染方式
	*	例如,渲染没有Mesh的特殊Shader,体积云等特效
	*
	* 注意:此通道不会管理其中Observer的生命周期
	*
	*/
	class TINY_API ReplacedPipelinePass : public PipelinePass
	{
		friend class BaseMeshRenderer;
		TINY_OBJECT_H(ReplacedPipelinePass, PipelinePass)
	protected:
		ReplacedPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32_t globalFunction = GLOBAL_NONE);

	public:
		virtual ~ReplacedPipelinePass();
		void addToPipeline() override;

		/*
		* 自定义剔除方式
		* 需要手动对所有渲染对象进行剔除 以及设置他们的渲染命令生成方法
		*/
		void setCustomCulling(const std::function<void(ReplacedPipelinePass*)>& func)
		{
			mIsAutoCulling = false;
			mCustomCulling = func;
		}

		/*
		* 自动剔除方式
		* 只需要设置渲染命令的生成方式即可
		*/
		void setAutoCulling(const std::function<RenderCommand* (BaseMeshRenderer*)>& func)
		{
			mIsAutoCulling = true;
			mAutoCulling = func;
		}

		void setAutoCulling()
		{
			mAutoCulling = TINY_BIND_THIS(ReplacedPipelinePass::createCommand);
		}

	private:
		void preCalculate();
		void pushCommand(BaseMeshRenderer* meshRenderer);

	private:
		RenderCommand* createCommand(BaseMeshRenderer* renderer);

	private:
		bool mIsAutoCulling;
		union
		{
			std::function<void(ReplacedPipelinePass*)> mCustomCulling;
			std::function<RenderCommand* (BaseMeshRenderer*)> mAutoCulling;
		};

	public:
		static void preRender();
		static void clearPassArray();

	private:
		static void addPreRenderArray(ReplacedPipelinePass* pass)
		{
			pass->saveObject();
			mPreRenderPassArray.push_back(pass);
		}

	private:
		static std::vector<ReplacedPipelinePass*> mPreRenderPassArray;
	};

	/*
	* 每一个queue都对应一个observer进行渲染
	* 如果是CamereObserver
	*/
	class TINY_API PipelineQueue : public PipelineWorker
	{
		PipelineQueue();
		TINY_OBJECT_H(PipelineQueue, PipelineWorker)

	public:
		virtual ~PipelineQueue();
		void addPipelinePass(PipelinePass* pass);

		void render();

		ObserverPipelinePass* createOrGetObserverPass(Shader* shader);

		void addToPipeline() override;
		void setObserver(BaseRenderObserver* observer) override;

		const int32_t getSortingID() const { return mSortingID; }

		void onEnterPipeline() override;
		void onExitPipeline() override;

	protected:
		void onClose() override;

	private:
		uint8_t mCountDown;
		int32_t mSortingID;
		bool mDirty;
		std::vector<PipelinePass*> mRenderingPassArray;
		std::vector<ObserverPipelinePass*> mPassTable;
	};
}
