#include "OctreeNode.h"
#include "Core/Component/OctreeAgent.h"
namespace tezcat::Tiny
{

	OctreeNode::OctreeNode(OctreeNode* parent, const OctPoint& point)
		: mOctPoint(point)
		, mLayerIndex(parent->mLayerIndex + 1)
		, mParent(parent)
	{

	}

	OctreeNode::OctreeNode()
		: mOctPoint(OctPoint::Root)
		, mLayerIndex(0)
	{

	}

	void OctreeNode::addAgent(OctreeAgent* agent)
	{
		// 	if (mObjects.count() < 2)
		// 	{
		// 		if (mObjects.count() == 1)
		// 		{
		// 			return;
		// 		}
		// 	}

			//最小了不分了
		glm::vec3 dimensions = mBoundingBox.getDimensions();
		if (dimensions.x < OctreeConfig::MinBounds
			&& dimensions.y < OctreeConfig::MinBounds
			&& dimensions.z < OctreeConfig::MinBounds)
		{
			agent->hostNode = this;
			mObjects.push(agent);
			return;
		}

		if (mChildren[0] == nullptr)
		{
			glm::vec3 center = mBoundingBox.min + (dimensions * 0.5f);

			mChildren[0] = new OctreeNode(this, OctPoint::NF1);
			mChildren[1] = new OctreeNode(this, OctPoint::NF2);
			mChildren[2] = new OctreeNode(this, OctPoint::NF3);
			mChildren[3] = new OctreeNode(this, OctPoint::NF4);

			mChildren[4] = new OctreeNode(this, OctPoint::NB1);
			mChildren[5] = new OctreeNode(this, OctPoint::NB2);
			mChildren[6] = new OctreeNode(this, OctPoint::NB3);
			mChildren[7] = new OctreeNode(this, OctPoint::NB4);

			/*
			*
			*   /-------/-------/|
			*  /	   /	   / |
			* /	      /	      /  |
			* |-------|-------|	 |
			* |	N2	  |	N1    |	/|
			* |		  |		  |/ |
			* |-------|-------|  |
			* |	N3	  |	N4	  |  |
			* |		  |		  | /
			* |-------|-------|/
			*
			* front
			*
			*/

			//front 4
			mChildren[0]->mBoundingBox.reset(center, mBoundingBox.max);

			mChildren[1]->mBoundingBox.reset(glm::vec3(mBoundingBox.min.x, center.y, center.z)
				, glm::vec3(center.x, mBoundingBox.max.y, mBoundingBox.max.z));

			mChildren[2]->mBoundingBox.reset(glm::vec3(mBoundingBox.min.x, mBoundingBox.min.y, center.z)
				, glm::vec3(center.x, center.y, mBoundingBox.max.z));

			mChildren[3]->mBoundingBox.reset(glm::vec3(center.x, mBoundingBox.min.y, center.z)
				, glm::vec3(mBoundingBox.max.x, center.y, mBoundingBox.max.z));

			//back 4
			mChildren[4]->mBoundingBox.reset(glm::vec3(center.x, center.y, mBoundingBox.min.z)
				, glm::vec3(mBoundingBox.max.x, mBoundingBox.max.y, center.z));

			mChildren[5]->mBoundingBox.reset(glm::vec3(mBoundingBox.min.x, center.y, mBoundingBox.min.z)
				, glm::vec3(center.x, mBoundingBox.max.y, center.z));

			mChildren[6]->mBoundingBox.reset(mBoundingBox.min, center);

			mChildren[7]->mBoundingBox.reset(glm::vec3(center.x, mBoundingBox.min.y, mBoundingBox.min.z)
				, glm::vec3(mBoundingBox.max.x, center.y, center.z));
		}

		//----------------------------------------------
		for (int i = 0; i < 8; i++)
		{
			if (mChildren[i]->mBoundingBox.contains(agent->boundingBox))
			{
				mChildren[i]->addAgent(agent);
				return;
			}
		}

		agent->hostNode = this;
		mObjects.push(agent);
	}

	void OctreeNode::log()
	{
		std::cout << OctreeConfig::getName(mOctPoint) << "|" << mLayerIndex << "|" << mObjects.count() << std::endl;
		if (mChildren[0] != nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				mChildren[i]->log();
			}
		}
	}

	void OctreeNode::logBoundingBox()
	{
		auto d = mBoundingBox.getDimensions();
		auto pos = mBoundingBox.getPosition();
		std::cout << "==========BoundingBox===========" << std::endl;
		std::cout << OctreeConfig::getName(mOctPoint) << std::endl;
		std::cout << "Min:" << mBoundingBox.min.x << "," << mBoundingBox.min.y << "," << mBoundingBox.min.z << std::endl;
		std::cout << "Max:" << mBoundingBox.max.x << "," << mBoundingBox.max.y << "," << mBoundingBox.max.z << std::endl;
		std::cout << "Size:" << d.x << "," << d.y << "," << d.z << std::endl;
		std::cout << "Position:" << pos.x << "," << pos.y << "," << pos.z << std::endl;
		std::cout << "================================" << std::endl;
	}
}
