#pragma once

#include "functional"
#include "vector"
#include "string"
#include "iostream"

namespace test 
{

	class Test
	{
	public:
		Test() {}
		virtual ~Test() {}

		virtual	void OnUpdata(float daltaTime) {}
		virtual	void OnRender() {}
		virtual	void OnImGuiRender() {}

	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);
		~TestMenu();

		void OnImGuiRender() override;

		template<typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Register Test: " << name << std::endl;
			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test*()>>> m_Tests;
	};

}
