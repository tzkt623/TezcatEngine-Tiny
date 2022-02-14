#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"


namespace tezcat::Tiny::Core
{
	class Camera;
	class PassShader;

	class TINY_API Pipeline
	{
	public:
		enum Queue : int
		{
			Background = 0,
			Opaque,
			Alpha,
			OpaqueLast,
			Transparent,
			Overlay
		};
	public:
		Pipeline(Queue queue) : m_QueueID(queue) {}

		inline int getQueueID() const { return m_QueueID; }
		inline void setQueueID(Queue queue) { m_QueueID = queue; }
		void render(Camera* camera);
		void addPass(PassShader* pass);

	private:
		int m_QueueID;
		std::vector<PassShader*> m_ShaderList;

	public:
		static Queue getQueue(const std::string& name)
		{
			return s_QueueMap[name];
		}

	private:
		static std::unordered_map<std::string, Queue> s_QueueMap;
	};

	class MeshRenderer;
	class TINY_API PipelineGroup
	{
	public:
		PipelineGroup();
		~PipelineGroup();

		void init();
		void render(Camera* camera);

		void addMeshRenderer(MeshRenderer* meshRenderer);
	private:
		std::vector<Pipeline*> m_PipelineList;
	};
}