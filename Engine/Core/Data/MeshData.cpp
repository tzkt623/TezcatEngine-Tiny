#include "MeshData.h"

namespace tezcat::Tiny
{
	MeshData::MeshData()
		: name()
		, drawMode(DrawMode::Triangles)
		, mChildrenData(nullptr)
	{

	}

	MeshData::MeshData(const std::string& name)
		: name(name)
		, drawMode(DrawMode::Triangles)
		, mChildrenData(nullptr)
	{

	}

	MeshData::MeshData(MeshData&& other) noexcept
		: name(std::move(other.name))
		, drawMode(DrawMode::Triangles)
		, vertices(std::move(other.vertices))
		, normals(std::move(other.normals))
		, colors(std::move(other.colors))
		, uvs(std::move(other.uvs))
		, indices(std::move(other.indices))
		, mChildrenData(other.mChildrenData)
	{
		other.mChildrenData = nullptr;
	}

	MeshData::~MeshData()
	{
		this->vertices.clear();
		this->normals.clear();
		this->colors.clear();
		this->uvs.clear();
		this->indices.clear();

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
		if (!this->vertices.empty())
		{
			count++;
		}

		if (!this->normals.empty())
		{
			count++;
		}

		if (!this->colors.empty())
		{
			count++;
		}

		if (!this->uvs.empty())
		{
			count++;
		}

		if (!this->indices.empty())
		{
			count++;
		}

		return count;
	}

	MeshData& MeshData::operator=(MeshData&& other) noexcept
	{
		this->name = std::move(other.name);
		this->vertices = std::move(other.vertices);
		this->normals = std::move(other.normals);
		this->colors = std::move(other.colors);
		this->uvs = std::move(other.uvs);
		this->indices = std::move(other.indices);
		this->mChildrenData = other.mChildrenData;

		other.mChildrenData = nullptr;

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

	const void* MeshData::getVertexData(const VertexPosition& position, size_t& outDataLength)
	{
		switch (position)
		{
		case VertexPosition::VP_Position:
			outDataLength = this->vertexSize();
			return this->vertices.data();
		case VertexPosition::VP_Normal:
			outDataLength = this->normalSize();
			return this->normals.data();
		case VertexPosition::VP_Color:
			outDataLength = this->colorSize();
			return this->colors.data();
		case VertexPosition::VP_UV:
			outDataLength = this->uvSize();
			return this->uvs.data();
		default:
			break;
		}

		outDataLength = 0;
		return nullptr;
	}

	const void* MeshData::getIndexData(size_t& outDataLength)
	{
		outDataLength = this->indexSize();
		return this->indices.data();
	}

	void MeshData::apply(DrawMode drawMode)
	{
		drawMode = drawMode;

		layoutPositions.emplace_back(VertexPosition::VP_Position);

		if (!this->normals.empty())
		{
			layoutPositions.emplace_back(VertexPosition::VP_Normal);
		}

		if (!this->uvs.empty())
		{
			layoutPositions.emplace_back(VertexPosition::VP_UV);
		}

		if (!this->colors.empty())
		{
			layoutPositions.emplace_back(VertexPosition::VP_Color);
		}
	}

}


