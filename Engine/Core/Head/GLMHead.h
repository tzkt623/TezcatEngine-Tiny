#pragma once
#define GLM_FORCE_RADIANS
#include "ThirdParty/glm/glm.hpp"
#include "ThirdParty/glm/gtc/type_ptr.hpp"

#include "ThirdParty/glm/gtc/matrix_transform.hpp"
#include "ThirdParty/glm/gtc/matrix_inverse.hpp"
#include "ThirdParty/glm/gtx/matrix_decompose.hpp"

#include "ThirdParty/glm/gtc/quaternion.hpp"
#include "ThirdParty/glm/gtx/quaternion.hpp"
#include "ThirdParty/glm/ext/quaternion_float.hpp"

namespace tezcat::Tiny
{
	using int2 = glm::ivec2;
	using int3 = glm::ivec3;
	using int4 = glm::ivec4;

	using float2 = glm::vec2;
	using float3 = glm::vec3;
	using float4 = glm::vec4;

	using float3x3 = glm::mat3;
	using float4x4 = glm::mat4;

	using quaternion = glm::quat;
}
