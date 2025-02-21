#pragma once
/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

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
#include "../Tool/Tool.h"


namespace tezcat::Tiny
{
	class ShadowCasterManager;
	class LightingManager;
	class LightManager;
	class RenderPass;
	class BaseRenderObserver;
	class RenderCommand;
	class Shader;
	class FrameBuffer;
	class Transform;

	/*
	* 渲染通道
	* 1.收集所有渲染命令以及所需的数据
	* 2.对渲染命令进行排序整理
	* 3.向当前Shader提交全局数据
	*
	* 必要数据
	* 1.Shader
	* 2.Observer
	*
	* 生命周期
	* 1.Pass的生命周期主要归RenderObserver
	* 2.副Pipeline只负责检测是否需要删除
	* 3.当Observer删除时,删除对应的Pass
	* 4.当一个Observer被添加到Pipeline中时,他下属的Pass均添加到Pipeline中
	*/
	class TINY_API PipelinePass : public TinyObject
	{
	public:
		enum class Mode : int8
		{
			//持续存在型
			Continued = 0,
			//只存在一次型
			Once
		};

		enum GlobalSubmit : uint32
		{
			GLOBAL_ALL = 0,
			NO_SHADOW = 1 << 0,
			NO_LIGHTING = 1 << 1,
			NO_ENVLIGHTING = 1 << 2,
			GLOBAL_NONE = std::numeric_limits<uint32>::max()
		};

		TINY_ABSTRACT_OBJECT_H(PipelinePass, TinyObject)
	protected:
		PipelinePass(uint32 globalSubmitMask);

	public:

		virtual ~PipelinePass();

		virtual void onEnterPipeline();
		virtual void onExitPipeline();

		void beginRender();
		void render();
		void endRender();

		void addGlobalSubmit(std::function<void(Shader*)>& function)
		{
			mGlobalSubmitArray.emplace_back(function);
		}

		void resetGlobalFunction(uint32 globalSubmitMask);

		//Mode
	public:
		void setContinuedMode() { mMode = Mode::Continued; }
		void setOnceMode() { mMode = Mode::Once; }
		const Mode getMode() const { return mMode; }
		void resetOnceMode() { mIsOnceModeExecuted = false; }

	public:
		uint32 getOrderID() const { return mType2.userID; }
		void setOrderID(uint16 value)
		{
			mType2.userID = value;
		}

		void setName(std::string name)
		{
			mName.assign(std::move(name));
		}

		/*
		* PipelineID: uint32(8-8-16)
		* 相机OrderID(-128-127)-队列OrderID(0-255)-PassOrderID(0-65535)
		*
		* 排序时:
		* 1.先排相机
		* 2.再排相机中的队列
		* 3.再排队列中的通道
		*/
		uint64 getPipelineOrderID() const { return mPipelineOrderID; }
		uint32 getUID() const;

		void addCommand(RenderCommand* cmd);

		template<class CMD, class... Args>
		void addCommand(Args&&... args)
		{
			mCommandArray.push_back(new CMD(TINY_FWD(args)...));
		}

		void setFrameBuffer(FrameBuffer* frameBuffer);

		void addToPipeline();
		void removeFromPipeline();

		bool isNeedRemoved() const { return mNeedRemoved && mIsInPipeline; }
		BaseRenderObserver* getObserver() const { return mRenderObserver; }

	protected:
		void setObserver(BaseRenderObserver* observer);
		void setShader(Shader* shader);

		virtual void onClose() override;

	protected:
		std::string mName;
		Mode mMode;
		bool mIsOnceModeExecuted;
		bool mIsInPipeline;
		bool mNeedRemoved;
		Shader* mShader;
		BaseRenderObserver* mRenderObserver;
		FrameBuffer* mFrameBuffer;
		std::vector<RenderCommand*> mCommandArray;
		std::vector<std::function<void(Shader*)>> mGlobalSubmitArray;

		union
		{
			uint64 mPipelineOrderID;

			struct Type1
			{
				uint64 renderType : 8;
				uint64 observerOrder : 8;
				uint64 frameBufferOrder : 16;
				uint64 shaderQueueID : 8;
				uint64 userID : 16;
			} mCombineOrderID;


			struct Type2
			{
				uint64 temp : 32;
				uint64 userID : 16;
				uint64 shaderQueueID : 8;
				uint64 observerOrder : 8;
			} mType2;
		};

	};

	/*
	* 观察者通道
	* 1.观察者主动遍历Layer,找到合适的Renderer并让其自动生成CMD并加入到Pass中
	* 2.在此模式下Pass和CMD都是自动生成
	*
	* 注意:此通道中Pass的生命周期归Observer管
	*/
	class TINY_API ObserverPipelinePass : public PipelinePass
	{
		friend class BaseRenderObserver;
		TINY_OBJECT_H_VISIT(private, ObserverPipelinePass, PipelinePass)
	private:
		ObserverPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32 globalFunction = GLOBAL_ALL);

	public:
		virtual ~ObserverPipelinePass();
	};

	/*
	* 代替通道
	* 1.Pass让其中的Observer进行遍历,然后进入正常流程
	* 2.此通道默认不使用Layer中的Renderer进行渲染,如果需要请调用setUseCullLayerData函数
	*
	* 注意:此通道中Observer生命周期归Pass管
	*/
	class TINY_API ReplacedPipelinePass : public PipelinePass
	{
		TINY_OBJECT_H(ReplacedPipelinePass, PipelinePass)
	protected:
		ReplacedPipelinePass(BaseRenderObserver* renderObserver
			, Shader* shader
			, uint32 globalFunction = GLOBAL_NONE);

	public:
		virtual ~ReplacedPipelinePass();
		void preCalculate();
		void setUseCullLayerData(bool val) { mUseCullLayer = val; }
		void setPreFunction(const std::function<void(ReplacedPipelinePass*)>& preFunction)
		{
			mPreFunction = preFunction;
		}

	private:
		bool mUseCullLayer;
		std::function<void(ReplacedPipelinePass*)> mPreFunction;
	};

#pragma region PipeQueue

	class TINY_API PipelineQueue : public TinyObject
	{
	public:


	private:
		std::vector<PipelinePass*> mPass;
	};

#pragma endregion




#pragma region Pipeline
	/*
	* 渲染管线
	*/
	class TINY_API Pipeline
	{
	public:
		Pipeline();
		virtual ~Pipeline() = default;

		virtual void init();
		virtual void render();
		virtual void addPipePass(PipelinePass* pass) = 0;

		static uint32 getFrameCount() { return sFrameCount; }

	protected:
		virtual void preRender() = 0;
		virtual void onRender() = 0;
		virtual void postRender() = 0;

	protected:
		static uint32 sFrameCount;
	};

	/*
	* 当管线开始渲染时,他需要确定一些东西
	* 1.环境光照,他需要一个6面正方形相机
	* 2.阴影效果,不同的灯光需要不同的相机
	* 3.游戏画面,玩家视口相机
	*	a.正向渲染
	*	b.延迟渲染
	*	c.后处理效果
	* 4.屏幕空间效果
	*
	* 所以管线的顺序应该可以让用户自己安排
	*/
	class TINY_API PipelineBuildin : public Pipeline
	{
	public:
		PipelineBuildin();
		virtual ~PipelineBuildin() noexcept;
		virtual void addPipePass(PipelinePass* pass) override;
	protected:
		virtual void preRender() override;
		virtual void onRender() override;
		virtual void postRender() override;

	protected:
		bool mDirty;
		std::vector<PipelinePass*> mPassArray;
		std::vector<ReplacedPipelinePass*> mReplacedPipePassArray;
	};


	class TINY_API PipelineHelper
	{
		PipelineHelper() = delete;
		~PipelineHelper() = delete;
	public:
		static void setPipeline(Pipeline* pipeline)
		{
			sPipeline = pipeline;
		}

		static void addPipePass(PipelinePass* pass)
		{
			sPipeline->addPipePass(pass);
		}

		Pipeline* getCurrentPipeline()
		{
			return sPipeline;
		}

	private:
		static Pipeline* sPipeline;
	};
#pragma endregion
}
