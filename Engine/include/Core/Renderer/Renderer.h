#pragma once
#include "Vertex.h"

#include "../Component/Component.h"
#include "../Head/RenderConfig.h"
#include "../Head/GLMHead.h"

#include "../Renderer/Material.h"


namespace tezcat::Tiny
{
	enum class TINY_API RenderObjectType
	{
		Observer,
		MeshRenderer,
		Light
	};

	class Shader;
	class FrameBuffer;
	class RenderQueue;
	class BaseGraphics;
	class Pipeline;
	class PipelinePass;
	class BaseRenderObserver;
	class Transform;


	/*
	* IRenderObject
	*
	* @brief 可渲染对象
	*/
	class TINY_API BaseRenderer : public TinyObject
	{
		TINY_ABSTRACT_OBJECT_H(BaseRenderer, TinyObject)

	protected:
		BaseRenderer() {};

	public:

		/*
		 * Author:	HCL
		 * Info:	2023|4|9
		 *
		 */
		virtual ~BaseRenderer() {};

		/*
		 * Info:		HCL|2023|4|8
		 * Access:
		 * Returns:
		 * Qualifier:
		 * Parameter:
		 * Comment:
		 */
		virtual RenderObjectType getRenderObjectType() = 0;

		/*
		* Info:		HCL|2023|4|8
		* Access:		virtual public
		* Returns:		void
		* Qualifier:
		* Parameter:	Shader * shader
		* Comment: 向shader提交数据
		*/
		virtual void submit(Shader* shader) = 0;


		virtual void makeRenderCommand(BaseRenderObserver* renderObserver) = 0;
		virtual void makeRenderCommand(PipelinePass* pass) = 0;
	};

	/*
	* IRenderMesh
	*
	* @brief 可渲染mesh
	*/
	class TINY_API BaseMeshRenderer : public BaseRenderer
	{
		TINY_ABSTRACT_OBJECT_H(BaseMeshRenderer, BaseRenderer)

	protected:
		BaseMeshRenderer();
	public:
		virtual ~BaseMeshRenderer();

		RenderObjectType getRenderObjectType() final { return RenderObjectType::MeshRenderer; }

		/*
		* @Author:	HCL
		* @Info:	2023|4|8
		* 材质
		*/
		Material* getMaterial() const { return mMaterial; }
		void setMaterial(Material* material);

		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 顶点数量
		*/
		int getVertexCount() const
		{
			return mVertex->getVertexCount();
		}

		/*
		* @Author:	HCL
		* @Info:	2023|4|9
		* 索引数量
		*/
		int getIndexCount() const
		{
			return mVertex->getIndexCount();
		}

		void setMesh(const std::string& meshName);
		void setMesh(MeshData* meshData);
		void setMesh(Vertex* vertex);
		Vertex* getVertex() const { return mVertex; }

		void submit(Shader* shader) override;

		void setShadowReciever(bool value) { mIsShadowReciever = true; }
		bool isShadowReciever() const { return mIsShadowReciever; }

		virtual void makeRenderCommand(BaseRenderObserver* renderObserver) override;
		virtual void makeRenderCommand(PipelinePass* pass) override;

		void setTransform(Transform* value) { mTransform = value; }
		Transform* getTransform() const { return mTransform; }

	protected:
		bool mIsShadowReciever;
		Material* mMaterial;
		Vertex* mVertex;
		Transform* mTransform;
	};


#define TINY_MESH_RENDER_FUNCTION(memberName)\
public:\
	void setMesh(const std::string& meshName) { memberName->setMesh(meshName); }\
	void setMesh(MeshData* meshData) { memberName->setMesh(meshData); }\
	void setMesh(Vertex* vertex) { memberName->setMesh(vertex); }\
	void setMaterial(Material* material) { memberName->setMaterial(material); }\
	void setShadowReciever(bool value) { memberName->setShadowReciever(value); }\
public:\
	Vertex* getVertex() const { return memberName->getVertex(); }\
	Material* getMaterial() const { return memberName->getMaterial(); }
}
