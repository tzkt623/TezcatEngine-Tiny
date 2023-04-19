#pragma once

#include "Tiny.h"

class MyObjectWindow;
class MyOverviewWindow;

struct ValueConfig
{
	float speed;
	float min;
	float max;
};



class MyGUIContext
{
public:
	static MyGUIContext& getInstance()
	{
		static MyGUIContext instance;
		return instance;
	}

	void selectObject(GameObject* object);

	void setOverviewWindow(MyOverviewWindow* window) { mOverviewWindow = window; }
	void setObjectWindow(MyObjectWindow* window) { mObjectWindow = window; }
	ValueConfig* getValueConfig(const UniformID& ID);

private:
	MyGUIContext();

	void initValueConfig();

	~MyGUIContext();


private:
	MyObjectWindow* mObjectWindow;
	MyOverviewWindow* mOverviewWindow;

	std::vector<ValueConfig*> mValueConfigAry;

public:
	static void matrix4(glm::mat4& mat4);
	static void transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

};

