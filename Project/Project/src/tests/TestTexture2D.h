#pragma once
#include "Test.h"

namespace test 
{
	class TestTexture2D : public Test
	{
	public:
		TestTexture2D();
		~TestTexture2D();

		void OnUpdata(float daltaTime) override;
		void OnRender() override;
		void OnImGuiRender() override;
	private:

	};
}

