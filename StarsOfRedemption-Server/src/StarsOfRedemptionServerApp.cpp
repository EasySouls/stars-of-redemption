#include "Walnut/Application.h"
#include "Walnut/EntryPoint.h"

#include "Walnut/Core/Log.h"

#include "ServerLayer.h"

Walnut::Application* Walnut::CreateApplication(int argc, char** argv)
{
	Walnut::ApplicationSpecification spec;
	spec.Name = "Stars of Redemption Server";

	Walnut::Application* app = new Walnut::Application(spec);
	app->PushLayer<StarsOfRedemption::ServerLayer>();
	return app;
}