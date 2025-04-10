#pragma once

/*
	Copyright (C) 2024 Tezcat(特兹卡特) tzkt623@qq.com

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/
#include "Vertex.h"

#include "../Component/Component.h"
#include "../Renderer/RenderConfig.h"
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
	class ReplacedPipelinePass;
	class BaseRenderObserver;
	class CameraObserver;
	class Transform;
	class PipelineQueue;


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

		virtual void submit(Shader* shader) {} ;

		virtual void makeRenderCommand(PipelineQueue* queue) = 0;
		virtual void makeRenderCommand(ReplacedPipelinePass* pass) = 0;
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

		void setShadowReciever(bool value) { mIsShadowReciever = true; }
		bool isShadowReciever() const { return mIsShadowReciever; }

		virtual void makeRenderCommand(PipelineQueue* queue) override;
		virtual void makeRenderCommand(ReplacedPipelinePass* pass) override;

		void setTransform(Transform* value) { mTransform = value; }
		Transform* getTransform() const { return mTransform; }

		void setLayer(GameObject* go);

	protected:
		bool mIsShadowReciever;
		Material* mMaterial;
		Vertex* mVertex;
		Transform* mTransform;
		uint32_t* mLayerID;
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
