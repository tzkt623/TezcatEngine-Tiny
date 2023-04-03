#include "FrustumCulling.h"

#include "../Head/GLMHead.h"
#include "../Component/Camera.h"
#include "../Component/Transform.h"


namespace tezcat::Tiny::Core
{
	void CullHelper::createFrustumWithCamera(Camera* camera, Frustum& frustum)
	{
		const float halfVSide = camera->getFar() * tanf(camera->getFOV() * 0.5f);
		const float halfHSide = halfVSide * camera->getAspect();
		const glm::vec3 frontMultFar = camera->getFar() * camera->getFront();

		auto world_position = camera->getTransform()->getWorldPosition();
		frustum.nearFace = { world_position + camera->getNear() * camera->getFront(), camera->getFront() };
		frustum.farFace = { world_position + frontMultFar, -camera->getFront() };
		frustum.rightFace = { world_position, glm::cross(camera->getUp(), frontMultFar + camera->getRight() * halfHSide) };
		frustum.leftFace = { world_position, glm::cross(frontMultFar - camera->getRight() * halfHSide, camera->getUp()) };
		frustum.topFace = { world_position, glm::cross(camera->getRight(), frontMultFar - camera->getUp() * halfVSide) };
		frustum.bottomFace = { world_position, glm::cross(frontMultFar + camera->getUp() * halfVSide, camera->getRight()) };
	}
}