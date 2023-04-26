#pragma once

#include "Tiny.h"

class MyObjectWindow;
class MyOverviewWindow;

struct ValueConfig
{
	bool isColor;
	float speed;
	float min;
	float max;

// 	ValueConfig()
// 		: isColor(false)
// 		, speed(0)
// 		, min(0)
// 		, max(0)
// 	{}


};



class MyGUIContext
{
public:
	static MyGUIContext& getInstance()
	{
		static MyGUIContext instance;
		return instance;
	}
	ValueConfig* getValueConfig(const UniformID& ID);

private:
	MyGUIContext();

	void initValueConfig();

	~MyGUIContext();


private:

	std::vector<ValueConfig*> mValueConfigAry;

public:
	static void matrix4(glm::mat4& mat4);
	static void transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

};

