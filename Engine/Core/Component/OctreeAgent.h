﻿#pragma once

#include "../Tool/Collision/BoundingBox.h"
#include "Component.h"


namespace tezcat::Tiny
{
	class OctreeNode;
	class TINY_API OctreeAgent : public ComponentT<OctreeAgent>
	{
		OctreeAgent();
		TINY_OBJECT_H(OctreeAgent, ComponentT<OctreeAgent>)

	public:
		virtual ~OctreeAgent();

		void update();
		void removeSelf();
		void forearchPossible(const std::function<void(OctreeAgent*)>& function);

	public:
		BoundingBox boundingBox;

		OctreeAgent* preAgent = nullptr;
		OctreeAgent* nextAgent = nullptr;
		OctreeNode* hostNode = nullptr;
	};

}
