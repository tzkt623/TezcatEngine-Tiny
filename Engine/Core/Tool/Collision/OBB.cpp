#include "OBB.h"

namespace tezcat::Tiny
{
	OBB::OBB()
		: mMin(std::numeric_limits<float>::max())
		, mMax(std::numeric_limits<float>::min())
	{

	}

	OBB::OBB(const glm::vec3& min, const glm::vec3& max)
		: mMin(min)
		, mMax(max)
	{

	}

	OBB::OBB(const OBB& other)
		: mMax(other.mMax)
		, mMin(other.mMin)
	{

	}

	OBB::~OBB()
	{

	}

	glm::vec3 OBB::getCenter()
	{
		glm::vec3 center;
		center.x = (mMin.x + mMax.x) * 0.5f;
		center.y = (mMin.y + mMax.y) * 0.5f;
		center.z = (mMin.z + mMax.z) * 0.5f;
		return center;
	}

	void OBB::getCorners(std::array<glm::vec3, 8>& array)
	{
		array[0] = { mMin.x, mMax.y, mMax.z };
		array[1] = { mMin.x, mMin.y, mMax.z };
		array[2] = { mMax.x, mMin.y, mMax.z };
		array[3] = { mMax.x, mMax.y, mMax.z };

		array[4] = { mMax.x, mMax.y, mMin.z };
		array[5] = { mMax.x, mMin.y, mMin.z };
		array[6] = { mMin.x, mMin.y, mMin.z };
		array[7] = { mMin.x, mMax.y, mMin.z };
	}

	bool OBB::impact(const OBB& other)
	{
		return ((mMin.x >= other.mMin.x) && (mMax.x <= other.mMax.x) || (other.mMin.x >= mMin.x) && (other.mMax.x <= mMax.x))
			&& ((mMin.y >= other.mMin.y) && (mMax.y <= other.mMax.y) || (other.mMin.y >= mMin.y) && (other.mMax.y <= mMax.y))
			&& ((mMin.z >= other.mMin.z) && (mMax.z <= other.mMax.z) || (other.mMin.z >= mMin.z) && (other.mMax.z <= mMax.z));
	}

	bool OBB::contain(const glm::vec3& point)
	{
		if (point.x < mMin.x) return false;
		if (point.y < mMin.y) return false;
		if (point.z < mMin.z) return false;
		if (point.x > mMax.x) return false;
		if (point.y > mMax.y) return false;
		if (point.z > mMax.z) return false;

		return true;
	}

	void OBB::combine(const OBB& other)
	{
		mMin = glm::min(mMin, other.mMin);
		mMax = glm::max(mMax, other.mMax);
	}

	void OBB::updateMinMax(const std::array<glm::vec3, 8>& array)
	{
		for (auto i : array)
		{
			//
			if (i.x < mMin.x)
			{
				mMin.x = i.x;
			}

			if (i.x > mMax.x)
			{
				mMax.x = i.x;
			}

			//
			if (i.y < mMin.y)
			{
				mMin.y = i.y;
			}

			if (i.y > mMax.y)
			{
				mMax.y = i.y;
			}

			//
			if (i.z < mMin.z)
			{
				mMin.z = i.z;
			}

			if (i.z > mMax.z)
			{
				mMax.z = i.z;
			}
		}
	}

	void OBB::transform(const glm::mat4& localToWorldMatrix)
	{
		std::array<glm::vec3, 8> array =
		{
			localToWorldMatrix * glm::vec4(mMin.x, mMax.y, mMax.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMin.x, mMin.y, mMax.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMax.x, mMin.y, mMax.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMax.x, mMax.y, mMax.z, 1.0f),

			localToWorldMatrix * glm::vec4(mMax.x, mMax.y, mMin.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMax.x, mMin.y, mMin.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMin.x, mMin.y, mMin.z, 1.0f),
			localToWorldMatrix * glm::vec4(mMin.x, mMax.y, mMin.z, 1.0f)
		};

		this->updateMinMax(array);
	}

	void OBB::reset()
	{
		mMin = glm::vec3(std::numeric_limits<float>::max());
		mMax = glm::vec3(std::numeric_limits<float>::min());
	}

	bool OBB::isValid()
	{
		return !(mMin.x > mMax.x || mMin.y > mMax.y || mMin.z > mMax.z);
	}
}
