#include "AABB.h"

namespace tezcat::Tiny
{
	AABB::AABB()
		: m_Min(std::numeric_limits<float>::max())
		, m_Max(std::numeric_limits<float>::min())
	{

	}

	AABB::AABB(const glm::vec3& min, const glm::vec3& max)
		: m_Min(min)
		, m_Max(max)
	{

	}

	AABB::AABB(const AABB& other)
		: m_Max(other.m_Max)
		, m_Min(other.m_Min)
	{

	}

	AABB::~AABB()
	{

	}

	glm::vec3 AABB::getCenter()
	{
		glm::vec3 center;
		center.x = (m_Min.x + m_Max.x) * 0.5f;
		center.y = (m_Min.y + m_Max.y) * 0.5f;
		center.z = (m_Min.z + m_Max.z) * 0.5f;
		return center;
	}

	void AABB::getCorners(std::array<glm::vec3, 8>& array)
	{
		array[0] = { m_Min.x, m_Max.y, m_Max.z };
		array[1] = { m_Min.x, m_Min.y, m_Max.z };
		array[2] = { m_Max.x, m_Min.y, m_Max.z };
		array[3] = { m_Max.x, m_Max.y, m_Max.z };

		array[4] = { m_Max.x, m_Max.y, m_Min.z };
		array[5] = { m_Max.x, m_Min.y, m_Min.z };
		array[6] = { m_Min.x, m_Min.y, m_Min.z };
		array[7] = { m_Min.x, m_Max.y, m_Min.z };
	}

	bool AABB::impact(const AABB& other)
	{
		return ((m_Min.x >= other.m_Min.x) && (m_Max.x <= other.m_Max.x) || (other.m_Min.x >= m_Min.x) && (other.m_Max.x <= m_Max.x))
			&& ((m_Min.y >= other.m_Min.y) && (m_Max.y <= other.m_Max.y) || (other.m_Min.y >= m_Min.y) && (other.m_Max.y <= m_Max.y))
			&& ((m_Min.z >= other.m_Min.z) && (m_Max.z <= other.m_Max.z) || (other.m_Min.z >= m_Min.z) && (other.m_Max.z <= m_Max.z));
	}

	bool AABB::contain(const glm::vec3& point)
	{
		if (point.x < m_Min.x) return false;
		if (point.y < m_Min.y) return false;
		if (point.z < m_Min.z) return false;
		if (point.x > m_Max.x) return false;
		if (point.y > m_Max.y) return false;
		if (point.z > m_Max.z) return false;

		return true;
	}

	void AABB::combine(const AABB& other)
	{
		m_Min = glm::min(m_Min, other.m_Min);
		m_Max = glm::max(m_Max, other.m_Max);
	}

	void AABB::updateMinMax(const std::array<glm::vec3, 8>& array)
	{
		for (auto i : array)
		{
			//
			if (i.x < m_Min.x)
			{
				m_Min.x = i.x;
			}

			if (i.x > m_Max.x)
			{
				m_Max.x = i.x;
			}

			//
			if (i.y < m_Min.y)
			{
				m_Min.y = i.y;
			}

			if (i.y > m_Max.y)
			{
				m_Max.y = i.y;
			}

			//
			if (i.z < m_Min.z)
			{
				m_Min.z = i.z;
			}

			if (i.z > m_Max.z)
			{
				m_Max.z = i.z;
			}
		}
	}

	void AABB::transform(const glm::mat4& localToWorldMatrix)
	{
		std::array<glm::vec3, 8> array =
		{
			localToWorldMatrix * glm::vec4(m_Min.x, m_Max.y, m_Max.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Min.x, m_Min.y, m_Max.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Max.x, m_Min.y, m_Max.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Max.x, m_Max.y, m_Max.z, 1.0f),

			localToWorldMatrix * glm::vec4(m_Max.x, m_Max.y, m_Min.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Max.x, m_Min.y, m_Min.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Min.x, m_Min.y, m_Min.z, 1.0f),
			localToWorldMatrix * glm::vec4(m_Min.x, m_Max.y, m_Min.z, 1.0f)
		};

		this->updateMinMax(array);
	}

	void AABB::reset()
	{
		m_Min = glm::vec3(std::numeric_limits<float>::max());
		m_Max = glm::vec3(std::numeric_limits<float>::min());
	}

	bool AABB::isValid()
	{
		return !(m_Min.x > m_Max.x || m_Min.y > m_Max.y || m_Min.z > m_Max.z);
	}
}