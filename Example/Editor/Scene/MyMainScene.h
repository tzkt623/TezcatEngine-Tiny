#pragma once
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

#include "include/Tiny.h"


class MyMainScene : public Scene
{
	MyMainScene(const std::string& name);
	TINY_OBJECT_H(MyMainScene, Scene)

public:
	void createGates(float gateWidth, float gateHigh);
	void createPaintings();
	void createPlane();
	void createCubes0();
	void createTransparentObject();
	void createInfinitePlane();
	void createDirectionLight();
	void createPBR();
	void createModel();

protected:
	virtual void onEnter() override;
	virtual void onExit() override;

private:
	GameObject* mController;
public:
};

