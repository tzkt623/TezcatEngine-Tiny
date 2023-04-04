#pragma once
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"
#include "../Head/Context.h"

namespace tezcat::Tiny::Core
{
	class Shader;
	class PassVertexGroup;
	class Camera;
	class MeshRenderer;
	class ILight;
	class IRenderObject;


	/// <summary>
	/// 
	/// </summary>
	class TINY_API RenderPass
	{
	public:
		RenderPass(Shader* shader);
		RenderPass(Shader* shader, ILight* light);
		RenderPass(Shader* shader, ILight* light, DrawMode drawMode);
		~RenderPass();

		void addRenderObject(IRenderObject* renderObject);

		inline bool isAttached() { return mInPipeline; }
		inline void attach() { mInPipeline = true; }
		inline void detach() { mInPipeline = false; }

		ILight* getLight() const { return mLight; }
		void setLight(ILight* val) { mLight = val; }

	public:
		void render(Camera* camera);

		int getOrderID() const;
		int getProgramID() const;
		const std::string& getName() const;

		Shader* getShader();

	private:
		bool mInPipeline;
		Shader* mShader;
		ILight* mLight;

		std::vector<IRenderObject*> mRenderObjects;
		std::unordered_map<uint32_t, PassVertexGroup*> mVAOWithID;
	};
}

