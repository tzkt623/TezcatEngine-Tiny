#include "MeshData.h"


namespace tezcat::Tiny
{
	TINY_RTTI_CPP(MeshData);
	MeshData::MeshData()
		: MeshData("")
	{

	}

	MeshData::MeshData(const std::string& name)
		: mName(name)
		, mDrawMode(DrawMode::Triangles)
	{

	}

	MeshData::~MeshData()
	{
		this->mVertices.clear();
		this->mNormals.clear();
		this->mColors.clear();
		this->mUVs.clear();
		this->mIndices.clear();

		if (mChildrenData != nullptr)
		{
			for (auto c : *mChildrenData)
			{
				delete c;
			}

			delete mChildrenData;
		}
	}

	int MeshData::getBufferSize()
	{
		int count = 0;
		if (!this->mVertices.empty())
		{
			count++;
		}

		if (!this->mNormals.empty())
		{
			count++;
		}

		if (!this->mColors.empty())
		{
			count++;
		}

		if (!this->mUVs.empty())
		{
			count++;
		}

		if (!this->mIndices.empty())
		{
			count++;
		}

		return count;
	}

	void MeshData::addChild(MeshData* meshData)
	{
		if (mChildrenData == nullptr)
		{
			mChildrenData = new std::vector<MeshData*>();
		}

		mChildrenData->push_back(meshData);
	}

	std::tuple<size_t, const void*> MeshData::getVertexData(const VertexPosition& position)
	{
		switch (position)
		{
		case VertexPosition::VP_Position:
			return { this->vertexSize(), this->mVertices.data() };
		case VertexPosition::VP_Normal:
			return { this->normalSize(), this->mNormals.data() };
		case VertexPosition::VP_Color:
			return  { this->colorSize(), this->mColors.data() };
		case VertexPosition::VP_UV:
			return  { this->uvSize(), this->mUVs.data() };
		default: return { 0, nullptr };
		}
	}

	std::tuple<size_t, const void*> MeshData::getIndexData()
	{
		return { this->indexSize(), this->mIndices.data() };
	}

	void MeshData::apply(DrawMode drawMode)
	{
		drawMode = drawMode;

		mLayoutPositions.emplace_back(VertexPosition::VP_Position);

		if (!this->mNormals.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_Normal);
		}

		if (!this->mUVs.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_UV);
		}

		if (!this->mColors.empty())
		{
			mLayoutPositions.emplace_back(VertexPosition::VP_Color);
		}
	}

	//--------------------------------------------------
	//
	//	ModelNode
	//
	ModelNode::ModelNode()
		: mChildrenData(nullptr)
		, mMeshData(nullptr)
	{

	}

	ModelNode::~ModelNode()
	{
		if (mChildrenData)
		{
			for (auto child : *mChildrenData)
			{
				delete child;
			}

		}
		delete mChildrenData;
	}

	void ModelNode::addChild(ModelNode* node)
	{
		if (mChildrenData == nullptr)
		{
			mChildrenData = new std::vector<ModelNode*>();
		}

		mChildrenData->push_back(node);
	}


	Model::Model()
		: mName()
	{

	}

	Model::~Model()
	{

	}

}
