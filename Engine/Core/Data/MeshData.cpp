#include "MeshData.h"


namespace tezcat::Tiny
{
	MeshData::MeshData()
		: MeshData("", -1)
	{

	}

	MeshData::MeshData(const std::string& name)
		: MeshData(name, -1)
	{

	}

	MeshData::MeshData(const std::string& name, const int& index)
		: mName(name)
		, mDrawMode(DrawMode::Triangles)
		, mChildrenData(nullptr)
		, mIndex(index)
	{

	}

	MeshData::MeshData(MeshData&& other) noexcept
		: mName(std::move(other.mName))
		, mDrawMode(DrawMode::Triangles)
		, mVertices(std::move(other.mVertices))
		, mNormals(std::move(other.mNormals))
		, mColors(std::move(other.mColors))
		, mUVs(std::move(other.mUVs))
		, mIndices(std::move(other.mIndices))
		, mChildrenData(other.mChildrenData)
		, mIndex(other.mIndex)
	{
		other.mIndex = -1;
		other.mChildrenData = nullptr;
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

	MeshData& MeshData::operator=(MeshData&& other) noexcept
	{
		this->mName = std::move(other.mName);
		this->mVertices = std::move(other.mVertices);
		this->mNormals = std::move(other.mNormals);
		this->mColors = std::move(other.mColors);
		this->mUVs = std::move(other.mUVs);
		this->mIndices = std::move(other.mIndices);
		this->mChildrenData = other.mChildrenData;
		this->mIndex = other.mIndex;

		other.mChildrenData = nullptr;
		other.mIndex = -1;

		return *this;
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
