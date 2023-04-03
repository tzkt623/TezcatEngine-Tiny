#pragma once

#include "../Component/Component.h"
#include "../Head/GLMHead.h"
#include "../Head/CppHead.h"
#include "../Head/ConfigHead.h"

namespace tezcat::Tiny::Core
{
	class TINY_API AABB : public ComponentT<AABB>
	{
	public:
		AABB();
		AABB(const glm::vec3& min, const glm::vec3& max);
		AABB(const AABB& other);
		~AABB();

		glm::vec3 getCenter();
		void getCorners(std::array<glm::vec3, 8>& array);
		bool impact(const AABB& other);
		bool contain(const glm::vec3& point);
		void combine(const AABB& other);
		void updateMinMax(const std::array<glm::vec3, 8>& array);
		void transform(const glm::mat4& localToWorldMatrix);
		void reset();
		bool isValid();

	private:
		glm::vec3 m_Min;
		glm::vec3 m_Max;
	};
}