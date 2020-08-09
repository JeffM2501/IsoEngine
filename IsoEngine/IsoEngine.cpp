// IsoEngine.cpp : Defines the entry point for the application.
//

////////////////////////////////////////////////////////////

#include "IsoEngine.h"
#include "IsoMap.h"

RenderWindowPtr WindowPtr;

void PollEvents();
bool SetupWindow();

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
	if (!SetupWindow())
		return -1;

	IsoMap map(WindowPtr);

	if (!map.LoadResources() || !map.LoadMap())
		return -1;

	while (WindowPtr->isOpen())
	{
		PollEvents();

		map.SetMousePostion(sf::Mouse::getPosition(*WindowPtr));

		WindowPtr->clear(sf::Color::Black);
		map.Draw();
		WindowPtr->display();
	}

	SpriteFactory::Cleanup();
	map.Cleanup();
	WindowPtr = nullptr;

	return 0;
}

bool SetupWindow()
{
	WindowPtr = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080, 32), "Isometric Game", sf::Style::Titlebar | sf::Style::Close);
	WindowPtr->setVerticalSyncEnabled(true);
	return true;
}

void PollEvents()
{
	sf::Event sfEvent;
	while (WindowPtr->pollEvent(sfEvent))
	{
		switch (sfEvent.type)
		{
		case sf::Event::Closed:
			WindowPtr->close();
			break;

		default:
			break;
		}
	}
}


