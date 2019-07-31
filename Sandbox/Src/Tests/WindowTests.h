#pragma once

#include <gtest/gtest.h>
#include "Engine/Core/Window.h"

class WindowTest: public testing::Test
{

protected:
	std::unique_ptr<ee::Window> m_Window;

};

TEST_F(WindowTest, CheckWindowInit)
{
	ee::WindowProps wp = ee::WindowProps(std::string("Test Engine"), 600, 480);
	m_Window = std::unique_ptr<ee::Window>(ee::Window::Create(wp));

	EXPECT_EQ(m_Window->GetHeight(), wp.Height);
	EXPECT_EQ(m_Window->GetWidth(), wp.Width);

}

