#pragma once

#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Camera;
	class RenderPass;
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
		PipelineQueue(Queue queue, uint32_t baseOrderID)
			: mQueueID(queue)
			, mBaseOrderID(baseOrderID)
			, mDirty(true)
		{
		}

		int getQueueID() const { return mQueueID; }
		void setQueueID(Queue queue) { mQueueID = queue; }
		virtual void render(Camera* camera);
		void addPass(RenderPass* pass);

	private:
		Queue mQueueID;
		uint32_t mBaseOrderID;
		bool mDirty;
		std::vector<RenderPass*> mShaderList;

	public:
		static Queue getQueue(const std::string& name)
		{
			return sQueueMap[name];
		}

	private:
		static std::unordered_map<std::string, Queue> sQueueMap;
	};
}
