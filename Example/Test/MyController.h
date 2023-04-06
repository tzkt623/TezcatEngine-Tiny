#pragma once

#include "Tiny.h"

class MyController : public ComponentT<MyController>
{
public:
	MyController();
	~MyController();

public:
	void yawPitch(float yaw, float pitch, bool constrainPitch = true);
	void updateCameraVector();

	glm::vec3& getFront() { return mFront; }
	glm::vec3& getUp() { return mUp; }
	glm::vec3& getRight() { return mRight; }

protected:
	void onStart() override;
	void onEnable() override;

	void warp();


private:
	glm::vec3 mFront;
	glm::vec3 mUp;
	glm::vec3 mRight;
	glm::vec3 mWorldUp;
	float mYaw;
	float mPitch;
	float mRoll;

	Transform* mWorld1Gate;
	Transform* mWorld2Gate;
	int mWorldIndex = 0;
};

