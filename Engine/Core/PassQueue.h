#pragma once
#include <queue>
#include <list>
#include <string>

namespace tezcat::Tiny::Core
{
	class Pass;
	class PassQueue
	{
	public:
		PassQueue(const std::string& name, int orderID);
		~PassQueue();

		std::string getName() const { return m_Name; }
		int getOrderID() const { return m_OrderID; }

	public:
		void addPass(Pass* pass);
		void update();

	private:
		std::string m_Name;
		std::list<Pass*> m_Passes;
		int m_OrderID;
	};
}