/*
	Copyright (C) 2022 - 2025 Tezcat(特兹卡特) tzkt623@qq.com

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

#include "Core/Tool/Collision/FrustumCulling.h"

#include "Core/Head/GLMHead.h"
#include "Core/Renderer/Renderer.h"
#include "Core/Component/Transform.h"
#include "Core/Renderer/RenderObserver.h"


namespace tezcat::Tiny
{
	void CullHelper::createFrustumWithCamera(BaseRenderObserver* camera, Frustum& frustum)
	{
		auto transform = dynamic_cast<Component*>(camera)->getTransform();
		const float halfVSide = camera->getFar() * tanf(camera->getFOV() * 0.5f);
		const float halfHSide = halfVSide * camera->getAspect();
		const float3 frontMultFar = camera->getFar() * transform->getForward();

		auto world_position = transform->getWorldPosition();
		frustum.nearFace = { world_position + camera->getNear() * transform->getForward(), transform->getForward() };
		frustum.farFace = { world_position + frontMultFar, -transform->getForward() };
		frustum.rightFace = { world_position, glm::cross(transform->getUp(), frontMultFar + transform->getRight() * halfHSide) };
		frustum.leftFace = { world_position, glm::cross(frontMultFar - transform->getRight() * halfHSide, transform->getUp()) };
		frustum.topFace = { world_position, glm::cross(transform->getRight(), frontMultFar - transform->getUp() * halfVSide) };
		frustum.bottomFace = { world_position, glm::cross(frontMultFar + transform->getUp() * halfVSide, transform->getRight()) };
	}
}
