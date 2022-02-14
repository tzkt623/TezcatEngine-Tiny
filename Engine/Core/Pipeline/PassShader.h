#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class VertexGroup;
	class PassVertexGroup;
	class Camera;
	class MeshRenderer;

	/// <summary>
	/// Pass是由Shader来生成的
	/// 使用同一个Shader的Object会被放在一起渲染
	/// </summary>
	class TINY_API PassShader
	{
	public:
		PassShader(Shader* program);
		~PassShader();

		void addMeshRenderer(MeshRenderer* meshRenderer);

		inline bool isAttached() { return m_InPipeline; }
		inline void attach() { m_InPipeline = true; }
		inline void detach() { m_InPipeline = false; }

	public:
		void render(Camera* camera);

		inline int getOrderID() const;
		inline int getProgramID() const;
		inline const std::string& getName() const;

	private:
		bool m_InPipeline;
		Shader* m_Shader;
		std::unordered_map<VertexGroup*, PassVertexGroup*> m_VAOWithID;
	};
}

