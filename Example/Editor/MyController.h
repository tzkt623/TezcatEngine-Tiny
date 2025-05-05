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

#pragma once

#include "include/Tiny.h"

class MyController : public ComponentAutoID<MyController>
{
public:
	MyController();
	~MyController();

public:
	void yawPitch(float yaw, float pitch, bool constrainPitch = true);
	void roll(float roll);
	void updateVector(Transform* transform);

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

