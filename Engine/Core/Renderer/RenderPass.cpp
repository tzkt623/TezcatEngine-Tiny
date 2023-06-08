#include "RenderPass.h"

#include "Vertex.h"
#include "BaseGraphics.h"
#include "RenderObject.h"
#include "RenderCommand.h"
#include "FrameBuffer.h"

#include "../Head/GLMHead.h"

#include "../Shader/Shader.h"

#include "../Manager/EnvironmentLightManager.h"
#include "../Manager/LightManager.h"
#include "../Manager/TextureManager.h"
#include "../Manager/CameraManager.h"
#include "../Manager/ShadowCasterManager.h"

#include "../Component/Camera.h"
#include "../Component/Transform.h"
#include "../Component/MeshRenderer.h"
#include "../Component/Light.h"

#include "../Profiler.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(RenderPass);

	RenderPass::RenderPass()
		: mShader(nullptr)
		, mDirty(false)
	{

	}

	RenderPass::RenderPass(Shader* shader)
		: mShader(shader)
		, mDirty(false)
	{
		//Pipeline::addPassStatic(this);
	}

	RenderPass::~RenderPass()
	{
		mShader = nullptr;
	}

	int RenderPass::getOrderID() const
	{
		return mShader->getOrderID();
	}

	int RenderPass::getProgramID() const
	{
		return mShader->getProgramID();
	}

	const std::string& RenderPass::getName() const
	{
		return mShader->getName();
	}

	void RenderPass::addCommand(RenderCommand* cmd)
	{
		mDirty = true;
		mCommandList.push_back(cmd);
	}

	bool RenderPass::checkState()
	{
		if (mCommandList.empty())
		{
			return false;
		}

		mShader->resetGlobalState();

		Profiler_PassCount(1);
		Profiler_DrawCall(static_cast<int>(mCommandList.size()));

		return true;
	}

	void RenderPass::sortRenderObjects(const std::function<bool(RenderCommand* a, RenderCommand* b)>& function)
	{
		std::ranges::sort(mCommandList, function);
	}

	void RenderPass::render(BaseGraphics* graphics)
	{
		if (mDirty)
		{
			mDirty = false;
			std::ranges::sort(mCommandList, [](RenderCommand* a, RenderCommand* b)
			{
				return a->mOrderID < b->mOrderID;
			});
		}

		//分析shader的光照模式
		switch (mShader->getLightMode())
		{
			//无光照
		case LightMode::Unlit:
			break;
			//前向光照模式
		case LightMode::Forward:
		{
			graphics->getLightManager()->getDirectionalLight()->submit(graphics, mShader);
			graphics->getShadowCasterManager()->submit(graphics, mShader);
			graphics->getEnvLitManager()->submit(graphics, mShader);
			break;
		}
		case LightMode::ForwardAdd:
			break;
		case LightMode::Deferred:
			break;
		default:
			break;
		}

		for (auto cmd : mCommandList)
		{
			cmd->run(graphics, mShader);
			delete cmd;
		}

		mCommandList.clear();
	}


	//--------------------------------------------------------------------
	//
	//	RenderPassQueue
	//
	RenderQueue::RenderQueue(IRenderObserver* observer)
		: mObserver(observer)
	{

	}

	RenderQueue::~RenderQueue()
	{
		mObserver = nullptr;

		for (auto pass : mPrepareAry)
		{
			if (pass)
			{
				pass->subRef();
			}
		}
		mPrepareAry.clear();
	}



	//--------------------------------------------------------------------
	//
	//	ExtraPassQueue
	//
	ExtraQueue::~ExtraQueue()
	{

	}

	void ExtraQueue::render(BaseGraphics* graphics)
	{
		FrameBuffer::bind(graphics, mObserver->getFrameBuffer());
		graphics->setViewport(mObserver->getViewportInfo());
		graphics->clear(mObserver->getClearOption());

		for (auto pass : mPasses)
		{
			if (pass->checkState())
			{
				auto shader = pass->getShader();
				graphics->bind(shader);
				graphics->setPassState(shader);

				mObserver->submit(graphics, shader);
				mObserver->submitViewMatrix(graphics, shader);

				pass->render(graphics);
			}
		}

		FrameBuffer::unbind(graphics, mObserver->getFrameBuffer());
	}

	void ExtraQueue::addRenderCommand(RenderCommand* cmd)
	{
		auto shader = cmd->getShader();
		auto uid = shader->getUID();

		if (uid >= mPrepareAry.size())
		{
			mPrepareAry.resize(uid + 1, nullptr);
		}

		if (mPrepareAry[uid] == nullptr)
		{
			auto pass = RenderPass::create(shader);
			pass->addRef();
			mPasses.push_back(pass);
			mPrepareAry[uid] = pass;
		}

		mPrepareAry[uid]->addCommand(cmd);
	}

	//--------------------------------------------------------------------
	//
	//	BasePassQueue
	//
	BaseQueue::~BaseQueue()
	{

	}

	void BaseQueue::render(BaseGraphics* graphics)
	{
		FrameBuffer::bind(graphics, mObserver->getFrameBuffer());

		graphics->setViewport(mObserver->getViewportInfo());
		graphics->clear(mObserver->getClearOption());

		if ((mObserver->getClearOption() & ClearOption::CO_Skybox) == ClearOption::CO_Skybox)
		{
			auto cmd = EnvLitMgr::getInstance()->createSkyboxCMD(graphics);
			this->addRenderCommand(cmd);
		}

		this->render(graphics, mBackground);
		this->render(graphics, mOpaque);
		this->render(graphics, mAlpha);
		this->render(graphics, mTransparent);
		this->render(graphics, mOpaqueLast);
		this->render(graphics, mOverlay);

		FrameBuffer::unbind(graphics, mObserver->getFrameBuffer());
	}

	void BaseQueue::render(BaseGraphics* graphics, std::vector<RenderPass*>& passes)
	{
		for (auto pass : passes)
		{
			if (pass->checkState())
			{
				auto shader = pass->getShader();
				graphics->bind(shader);
				graphics->setPassState(shader);

				mObserver->submit(graphics, shader);
				mObserver->submitViewMatrix(graphics, shader);

				pass->render(graphics);
			}
		}
	}

	void BaseQueue::addRenderCommand(RenderCommand* cmd)
	{
		auto shader = cmd->getShader();
		auto uid = shader->getUID();

		if (uid >= mPrepareAry.size())
		{
			mPrepareAry.resize(uid + 1, nullptr);
		}

		if (mPrepareAry[uid] == nullptr)
		{
			auto pass = RenderPass::create(shader);

			switch (shader->getRenderQueue())
			{
			case Queue::Background:
				mBackground.push_back(pass);
				break;
			case Queue::Opaque:
				mOpaque.push_back(pass);
				break;
			case Queue::AlphaTest:
				mAlpha.push_back(pass);
				break;
			case Queue::Transparent:
				mTransparent.push_back(pass);
				break;
			case Queue::OpaqueLast:
				mOpaqueLast.push_back(pass);
				break;
			case Queue::Overlay:
				mOverlay.push_back(pass);
				break;
			default:
				break;
			}

			mPrepareAry[uid] = pass;
			pass->addRef();
		}

		mPrepareAry[uid]->addCommand(cmd);
	}
}
