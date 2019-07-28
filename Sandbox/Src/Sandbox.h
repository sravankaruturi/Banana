#pragma once
#include "Engine.h"

class Sandbox : public ee::Application
{

public:
	Sandbox()
	{
		EE_TRACE("Hello!");
	}

};

ee::Application* ee::CreateApplication()
{
	return new Sandbox();
}