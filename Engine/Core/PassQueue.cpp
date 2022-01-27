#include "PassQueue.h"
#include "Pass.h"

namespace tezcat::Tiny::Core
{
	PassQueue::PassQueue(const std::string& name, int orderID):
		m_Name(name),
		m_OrderID(orderID)
	{

	}

	PassQueue::~PassQueue()
	{
	}

	void PassQueue::addPass(Pass* pass)
	{
		m_Passes.push_back(pass);
	}

	void PassQueue::update()
	{
// 		Pass* current = nullptr;
// 		auto it = m_Passes.begin();
// 		while (it != m_Passes.end())
// 		{
// 			(*it++)->update();
// 		}
	}
}