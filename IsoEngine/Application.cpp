#include "Application.h"
#include "ResourceManager.h"

void Application::Setup(const Configuration& config)
{
	SetupDisplay(config);
	if (View == nullptr)
		return;

	if (LoadLevel(config.InitalMapName))
	{
		MapPtr->LoadResources();
		OnStartup();

		Timer.restart();
	}
}

void Application::Shutdown()
{
	if (ShutdownCallback != nullptr)
		ShutdownCallback(*this);

	if (View != nullptr && View->Window.isOpen())
		View->Window.close();

	SpriteFactory::Cleanup();
	if (MapPtr != nullptr)
		MapPtr->Cleanup();

	MapPtr = nullptr;
	View = nullptr;
}

bool Application::Run()
{
	if (View == nullptr || MapPtr == nullptr || !View->Window.isOpen())
		return false;

	FrameTime = Timer.getElapsedTime();
	Timer.restart();

	OnPreUpdate();
	Draw();
	OnPostUpdate();

	return true;
}

bool Application::LoadLevel(const std::string& name)
{
	MapPtr = std::make_shared<IsoMap>(View);

	if (MapPtr->LoadMap(name))
		return true;
	
	MapPtr = nullptr;
	return false;
}

void Application::SetupDisplay(const Configuration& config)
{
	uint32_t style = sf::Style::Titlebar | sf::Style::Close;;
	sf::VideoMode mode = sf::VideoMode(config.WindowSize.x, config.WindowSize.y, 32);
	if (config.FullScreen)
	{
		mode = sf::VideoMode::getDesktopMode();
		style = sf::Style::Fullscreen;
	}

	View = std::make_shared<Viewport>(mode, config.Title, style);
	View->Window.setVerticalSyncEnabled(config.VSync);
	if (config.FPSLimit > 0)
		View->Window.setFramerateLimit(config.FPSLimit);

	sf::Image icon;
	if (icon.loadFromFile(ResourceManager::GetResourcePath(config.IconResource)))
		View->Window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
}

void Application::OnStartup()
{
//	MapPtr->CenterMap();
	if (StartupCallback != nullptr)
		StartupCallback(*this);
}

void Application::OnPreUpdate()
{
	if (PreUpdateCallback != nullptr)
		PreUpdateCallback(*this, FrameTime.asSeconds());

	// tell the map the mouse position so it can have the tile under the cursor ready
	auto pos = sf::Mouse::getPosition(View->Window);
	MapPtr->SetMousePostion(pos);

	sf::Event sfEvent;
	while (View->Window.pollEvent(sfEvent))
	{
		View->HandleEvent(sfEvent);

		switch (sfEvent.type)
		{
		case sf::Event::Closed:
			View->Window.close();
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
		case sf::Event::EventType::MouseButtonPressed:
			if (sfEvent.mouseButton.button == sf::Mouse::Button::Left)
				MapPtr->SelectTile(MapPtr->GetTileUnderCursor());
			break;

		default:
			break;
		}
	}

	float speed = 1000 * FrameTime.asSeconds();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		MapPtr->MoveViewpoint(0, speed * -1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		MapPtr->MoveViewpoint(0, speed * 1);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		MapPtr->MoveViewpoint(speed * -1, 0);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		MapPtr->MoveViewpoint(speed * 1, 0);
}

void Application::OnPostUpdate()
{
	if (PostUpdateCallback != nullptr)
		PostUpdateCallback(*this, FrameTime.asSeconds());
}

void Application::OnShutdown()
{
	if (ShutdownCallback != nullptr)
		ShutdownCallback(*this);
}

void Application::Draw()
{
	View->Window.clear(sf::Color::Black);
	MapPtr->Draw();
	View->Window.display();
}
