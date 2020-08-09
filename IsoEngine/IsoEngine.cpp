// IsoEngine.cpp : Defines the entry point for the application.
//

////////////////////////////////////////////////////////////

#include "IsoEngine.h"
#include "IsoMap.h"

RenderWindowPtr WindowPtr;
std::shared_ptr<IsoMap> MapPtr;

void PollEvents();
bool SetupWindow();

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
	if (!SetupWindow())
		return -1;

	MapPtr = std::make_shared<IsoMap>(WindowPtr);

	if (MapPtr->LoadResources() && MapPtr->LoadMap("Maps/exampleMap.tmx"))
	{
		MapPtr->CenterMap();
		Timer.restart();

		while (WindowPtr->isOpen())
		{
			PollEvents();

			auto pos = sf::Mouse::getPosition(*WindowPtr);
			MapPtr->SetMousePostion(pos);

			WindowPtr->clear(sf::Color::Black);
			MapPtr->Draw();
			WindowPtr->display();
		}
	}
	else if (WindowPtr != nullptr)
		WindowPtr->close();

	SpriteFactory::Cleanup();
	MapPtr->Cleanup();

	MapPtr = nullptr;
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
	FrameTime = Timer.getElapsedTime();
	Timer.restart();

	sf::Event sfEvent;
	while (WindowPtr->pollEvent(sfEvent))
	{
		switch (sfEvent.type)
		{
		case sf::Event::Closed:
			WindowPtr->close();
			break;

		case sf::Event::EventType::KeyPressed:
			switch (sfEvent.key.code)
			{
			case sf::Keyboard::Key::Num1:
				MapPtr->CenterMap();
				break;

			default:
				break;
			}
			break;

		default:
			break;
		}
	}
	
	float speed = 1000 * FrameTime.asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		MapPtr->MoveViewpoint(0, speed * 1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		MapPtr->MoveViewpoint(0, speed * -1);
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		MapPtr->MoveViewpoint(speed * 1, 0);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		MapPtr->MoveViewpoint(speed * -1, 0);
}


