#include "MeshData.h"

namespace tezcat::Tiny::Core
{
	MeshData::MeshData()
		: name()
		, mChildrenData(nullptr)
	{

	}

	MeshData::MeshData(const std::string& name)
		: name(name)
		, mChildrenData(nullptr)
	{

	}

	MeshData::MeshData(MeshData&& other) noexcept
		: name(std::move(other.name))
		, vertices(std::move(other.vertices))
		, normals(std::move(other.normals))
		, colors(std::move(other.colors))
		, uv(std::move(other.uv))
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
		this->uv.clear();
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

		if (!this->uv.empty())
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
		this->uv = std::move(other.uv);
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

}


