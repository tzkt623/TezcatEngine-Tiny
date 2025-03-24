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
	class BaseMeshRenderer;
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

		enum GlobalSubmit : uint32_t
		{
			GLOBAL_ALL = 0,
			NO_SHADOW = 1 << 0,
			NO_LIGHTING = 1 << 1,
			NO_ENVLIGHTING = 1 << 2,
			GLOBAL_NONE = std::numeric_limits<uint32_t>::max()
		};

		TINY_ABSTRACT_OBJECT_H(PipelinePass, TinyObject)
	protected:
		PipelinePass(uint32_t globalSubmitMask);

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

		void resetGlobalFunction(uint32_t globalSubmitMask);

		//Mode
	public:
		void setContinuedMode() { mMode = Mode::Continued; }
		void setOnceMode() { mMode = Mode::Once; }
		const Mode getMode() const { return mMode; }
		void resetOnceMode() { mIsOnceModeExecuted = false; }
		std::string getMemoryInfo() override;

	public:
		uint32_t getOrderID() const { return mType2.userID; }

		/*
		* User Set Custom OrderID
		* Range [0, 65535]
		*/
		void setOrderID(uint16 value)
		{
			mType2.userID = value;
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
		uint64 getPipelineOrderID() const { return mPipelineOrderID; }
		uint32_t getUID() const;

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
			uint64_t mPipelineOrderID;

			struct Type1
			{
				uint64_t renderType : 8;
				uint64_t observerOrder : 8;
				uint64_t frameBufferOrder : 16;
				uint64_t shaderQueueID : 8;
				uint64_t userID : 16;
			} mCombineOrderID;


			struct Type2
			{
				uint64_t temp : 32;
				uint64_t userID : 16;
				uint64_t shaderQueueID : 8;
				uint64_t observerOrder : 8;
			} mType2;
		};

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

		ReplacedPipelinePass(Shader* shader
			, uint32_t globalFunction = GLOBAL_NONE);

	public:
		virtual ~ReplacedPipelinePass();

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
		void setAutoCulling(const std::function<RenderCommand*(BaseMeshRenderer*)>& func)
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
		static void addArray(ReplacedPipelinePass* pass)
		{
			pass->saveObject();
			mReplacedPipePassArray.push_back(pass);
		}

	private:
		static std::vector<ReplacedPipelinePass*> mReplacedPipePassArray;
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
		virtual void addPipelinePass(PipelinePass* pass) = 0;

		static uint32_t getFrameCount() { return sFrameCount; }

	protected:
		virtual void preRender() = 0;
		virtual void onRender() = 0;
		virtual void postRender() = 0;

	protected:
		static uint32_t sFrameCount;
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
		virtual void addPipelinePass(PipelinePass* pass) override;
	protected:
		virtual void preRender() override;
		virtual void onRender() override;
		virtual void postRender() override;

	protected:
		bool mDirty;
		std::vector<PipelinePass*> mPassArray;
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

		static void addPipelinePass(PipelinePass* pass)
		{
			sPipeline->addPipelinePass(pass);
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
