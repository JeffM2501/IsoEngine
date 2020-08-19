// IsoEngine.cpp : Defines the entry point for the application.
//

////////////////////////////////////////////////////////////

#include "IsoEngine.h"
#include "IsoMap.h"
#include "Application.h"
#include "ResourceManager.h"

sf::Time FrameTime;
sf::Clock Timer;

#ifdef SFML_SYSTEM_IOS
#include <SFML/Main.hpp>
#endif

// do correct main for OS type
#ifdef _WINDOWS
int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int  nCmdShow)
#else
int main ()
#endif
{
	Configuration config;
	config.IconResource = "icon.png";
	config.Title = "Isometric Game";
	config.InitalMapName = "Maps/exampleDeepMap.tmx";

	Application app;
	app.Setup(config);
	while (app.Run());

	app.Shutdown();

	return 0;
}
