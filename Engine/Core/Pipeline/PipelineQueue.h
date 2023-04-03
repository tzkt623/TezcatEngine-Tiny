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
			: m_QueueID(queue)
			, m_BaseOrderID(baseOrderID)
			, mDirty(true)
		{
		}

		inline int getQueueID() const { return m_QueueID; }
		inline void setQueueID(Queue queue) { m_QueueID = queue; }
		void render(Camera* camera);
		void addPass(RenderPass* pass);

	private:
		Queue m_QueueID;
		uint32_t m_BaseOrderID;
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