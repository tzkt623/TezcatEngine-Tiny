#pragma once
#include "../Head/TinyCpp.h"
#include "../Head/ConfigHead.h"
#include "../Head/RenderConfig.h"
#include "../Base/TinyObject.h"

namespace tezcat::Tiny
{
	class Shader;
	class PassVertexGroup;
	class Camera;
	class MeshRenderer;
	class ILight;
	class BaseGraphics;
	class IRenderObject;
	class IRenderMesh;
	class IRenderObserver;
	class RenderCommand;
	class FrameBuffer;

	/*
	* RenderPass
	*
	* @brief 用于存放进入当前Pass的所有RenderCommand,执行时遍历所有command进行渲染
	*/
	class TINY_API RenderPass : public TinyObject
	{
		RenderPass(Shader* shader);
		RenderPass();

		TINY_Factory(RenderPass);
		TINY_RTTI_H(RenderPass);

	public:
		virtual ~RenderPass();

	public:
		int getOrderID() const;
		int getProgramID() const;
		const std::string& getName() const;
		Shader* getShader() { return mShader; }
		void setShader(Shader* shader) { mShader = shader; }

	public:
		void addCommand(RenderCommand* cmd);
		void sortRenderObjects(const std::function<bool(RenderCommand* a, RenderCommand* b)>& function);
		void render(BaseGraphics* graphics);
		bool checkState();


	private:
		bool mDirty;
		Shader* mShader;
		std::vector<RenderCommand*> mCommandList;
	};


	class RenderQueue
	{
	public:
		RenderQueue(IRenderObserver* observer);
		~RenderQueue();

		virtual void render(BaseGraphics* graphics) = 0;
		virtual void addRenderCommand(RenderCommand* cmd) = 0;

		IRenderObserver* getRenderObserver() { return mObserver; }

	protected:
		IRenderObserver* mObserver;
		std::vector<RenderPass*> mPrepareAry;
	};

	class ExtraQueue : public RenderQueue
	{
	public:
		using RenderQueue::RenderQueue;
		virtual ~ExtraQueue();

		void render(BaseGraphics* graphics) override;
		void addRenderCommand(RenderCommand* cmd) override;


	private:
		std::vector<RenderPass*> mPasses;
	};


	class BaseQueue : public RenderQueue
	{
	public:
		using RenderQueue::RenderQueue;
		virtual ~BaseQueue();

		void render(BaseGraphics* graphics) override;
		void addRenderCommand(RenderCommand* cmd) override;

	private:
		void render(BaseGraphics* graphics, std::vector<RenderPass*>& passes);

	private:
		std::vector<RenderPass*> mBackground;
		std::vector<RenderPass*> mOpaque;
		std::vector<RenderPass*> mAlpha;
		std::vector<RenderPass*> mTransparent;
		std::vector<RenderPass*> mOpaqueLast;
		std::vector<RenderPass*> mOverlay;
	};
}
