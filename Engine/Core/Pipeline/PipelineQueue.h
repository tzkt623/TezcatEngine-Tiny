#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class RenderPass;
	class BaseGraphics;
	class TINY_API PipelineQueue
	{
	public:
		enum Queue : int
		{
			Background = 0,
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
		virtual void render(BaseGraphics* graphics, Camera* camera);

	private:
		bool mDirty;
		Queue mQueueID;
		uint32_t mBaseOrderID;
		std::vector<RenderPass*> mShaderList;

	public:
		inline static Queue getQueue(const std::string& name)
		{
			return sQueueMap[name];
		}

	private:
		static std::unordered_map<std::string, Queue> sQueueMap;
	};
}
