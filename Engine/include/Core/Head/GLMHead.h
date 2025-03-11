#pragma once

/*
	Copyright (C) 2025 Tezcat(特兹卡特) tzkt623@qq.com

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
