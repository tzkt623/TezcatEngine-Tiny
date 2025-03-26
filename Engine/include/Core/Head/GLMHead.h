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

	class GLMHelper
	{
	public:
		static bool decompose(float4x4 const& ModelMatrix, float3& Translation, float3& Rotation, float3& Scale)
		{
			using namespace glm;
			using T = float;
			float4x4 LocalMatrix(ModelMatrix);
			quaternion Orientation;

			// Normalize the matrix.
			if (epsilonEqual(LocalMatrix[3][3], static_cast<T>(0), epsilon<T>()))
				return false;

			for (length_t i = 0; i < 4; ++i)
				for (length_t j = 0; j < 4; ++j)
					LocalMatrix[i][j] /= LocalMatrix[3][3];

			// Next take care of translation (easy).
			Translation = float3(LocalMatrix[3]);
			LocalMatrix[3] = float4(0, 0, 0, LocalMatrix[3].w);

			float3 Row[3], Pdum3;

			// Now get scale and shear.
			for (length_t i = 0; i < 3; ++i)
				for (length_t j = 0; j < 3; ++j)
					Row[i][j] = LocalMatrix[i][j];

			// Compute X scale factor and normalize first row.
			Scale.x = length(Row[0]);// v3Length(Row[0]);
			Row[0] = detail::scale(Row[0], static_cast<T>(1));
			// Now, compute Y scale and normalize 2nd row.
			Scale.y = length(Row[1]);
			Row[1] = detail::scale(Row[1], static_cast<T>(1));
			// Next, get Z scale and normalize 3rd row.
			Scale.z = length(Row[2]);
			Row[2] = detail::scale(Row[2], static_cast<T>(1));

			// At this point, the matrix (in rows[]) is orthonormal.
			// Check for a coordinate system flip.  If the determinant
			// is -1, then negate the matrix and the scaling factors.
			Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
			if (dot(Row[0], Pdum3) < 0)
			{
				for (length_t i = 0; i < 3; i++)
				{
					Scale[i] *= static_cast<T>(-1);
					Row[i] *= static_cast<T>(-1);
				}
			}

			// Now, get the rotations out, as described in the gem.

			// FIXME - Add the ability to return either quaternions (which are
			// easier to recompose with) or Euler angles (rx, ry, rz), which
			// are easier for authors to deal with. The latter will only be useful
			// when we fix https://bugs.webkit.org/show_bug.cgi?id=23799, so I
			// will leave the Euler angle code here for now.

			// ret.rotateY = asin(-Row[0][2]);
			// if (cos(ret.rotateY) != 0) {
			//     ret.rotateX = atan2(Row[1][2], Row[2][2]);
			//     ret.rotateZ = atan2(Row[0][1], Row[0][0]);
			// } else {
			//     ret.rotateX = atan2(-Row[2][0], Row[1][1]);
			//     ret.rotateZ = 0;
			// }

			int i, j, k = 0;
			T root, trace = Row[0].x + Row[1].y + Row[2].z;
			if (trace > static_cast<T>(0))
			{
				root = sqrt(trace + static_cast<T>(1.0));
				Orientation.w = static_cast<T>(0.5) * root;
				root = static_cast<T>(0.5) / root;
				Orientation.x = root * (Row[1].z - Row[2].y);
				Orientation.y = root * (Row[2].x - Row[0].z);
				Orientation.z = root * (Row[0].y - Row[1].x);
			} // End if > 0
			else
			{
				static int Next[3] = { 1, 2, 0 };
				i = 0;
				if (Row[1].y > Row[0].x) i = 1;
				if (Row[2].z > Row[i][i]) i = 2;
				j = Next[i];
				k = Next[j];

#           ifdef GLM_FORCE_QUAT_DATA_XYZW
				int off = 0;
#           else
				int off = 1;
#           endif

				root = sqrt(Row[i][i] - Row[j][j] - Row[k][k] + static_cast<T>(1.0));

				Orientation[i + off] = static_cast<T>(0.5) * root;
				root = static_cast<T>(0.5) / root;
				Orientation[j + off] = root * (Row[i][j] + Row[j][i]);
				Orientation[k + off] = root * (Row[i][k] + Row[k][i]);
				Orientation.w = root * (Row[j][k] - Row[k][j]);
			} // End if <= 0

			Rotation = glm::degrees(glm::eulerAngles(Orientation));

			return true;
		}
	};
}
