#include "FrustumCulling.h"

#include "Core/Head/GLMHead.h"
#include "Core/Renderer/RenderObject.h"
#include "Core/Component/Transform.h"


namespace tezcat::Tiny
{
	void CullHelper::createFrustumWithCamera(IRenderObserver* camera, Frustum& frustum)
	{
		auto transform = dynamic_cast<Component*>(camera)->getTransform();
		const float halfVSide = camera->getFar() * tanf(camera->getFOV() * 0.5f);
		const float halfHSide = halfVSide * camera->getAspect();
		const glm::vec3 frontMultFar = camera->getFar() * transform->getForward();

		auto world_position = transform->getWorldPosition();
		frustum.nearFace = { world_position + camera->getNear() * transform->getForward(), transform->getForward() };
		frustum.farFace = { world_position + frontMultFar, -transform->getForward() };
		frustum.rightFace = { world_position, glm::cross(transform->getUp(), frontMultFar + transform->getRight() * halfHSide) };
		frustum.leftFace = { world_position, glm::cross(frontMultFar - transform->getRight() * halfHSide, transform->getUp()) };
		frustum.topFace = { world_position, glm::cross(transform->getRight(), frontMultFar - transform->getUp() * halfVSide) };
		frustum.bottomFace = { world_position, glm::cross(frontMultFar + transform->getUp() * halfVSide, transform->getRight()) };
	}
}
