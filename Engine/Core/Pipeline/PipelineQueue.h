#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny
{
	class Camera;
	class RenderPass;
	class BaseGraphics;
	class ILight;
	class IRenderObserver;
	class Shader;
	class TINY_API PipelineQueue
	{
	public:
		enum Queue : int
		{
			None = 0,
			Background,
			Geometry,
			AlphaTest,
			OpaqueLast,
			Transparent,
			Overlay
		};

	public:
		PipelineQueue(const Queue& queue, const uint32_t& baseOrderID);

		int getQueueID() const { return mQueueID; }
		void setQueueID(const Queue& queue) { mQueueID = queue; }
		void addPass(RenderPass* pass);

		void render(BaseGraphics* graphics, Camera* camera);
		void render(BaseGraphics* graphics, IRenderObserver* observer, ILight* light);

	private:
		void sort();

	private:
		bool mDirty;
		Queue mQueueID;
		uint32_t mBaseOrderID;
		std::vector<RenderPass*> mRenderPassAry;

	public:
		inline static Queue getQueue(const std::string& name)
		{
			return sQueueMap[name];
		}

	private:
		static std::unordered_map<std::string, Queue> sQueueMap;
	};
}
