#pragma once

#include "Tiny.h"

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
	~MyGUIContext();
	void initValueConfig();

private:

	std::vector<ValueConfig*> mValueConfigAry;

public:
	static void matrix4(glm::mat4& mat4);
	static void transform(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

};

class MyWindow : public GUIWindow
{
public:
	MyWindow(const std::string& name)
		: GUIWindow(name)
	{
	}

	virtual ~MyWindow()
	{
		mOnCloseCallback();
	}

	void setCloseCallback(const std::function<void()>& callback)
	{
		mOnCloseCallback = callback;
	}

protected:
	std::function<void()> mOnCloseCallback;
};

#define CreateInstanceH(class_name)\
	class_name();\
public:\
	virtual ~class_name();\
	static class_name* create(GUI* host);\
private:\
	static class_name* sInstance

#define CreateInstanceCPP(class_name)\
	class_name* class_name::create(GUI* host)\
	{\
		if (sInstance == nullptr)\
		{\
			sInstance = new class_name();\
			sInstance->open(host);\
		}\
		return sInstance;\
	}\
	class_name* class_name::sInstance = nullptr

#define DeleteInstance() TinyAssert(sInstance == this); sInstance = nullptr
