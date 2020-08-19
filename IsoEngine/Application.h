#pragma once

#include "IsoEngine.h"
#include "IsoMap.h"
#include "Viewport.h"
#include <functional>

class Configuration
{
public:
	sf::Vector2i WindowSize = sf::Vector2i(1920, 1080);
	bool FullScreen = false;
	bool VSync = false;

	int FPSLimit = -1;

	std::string Title = "Default App";
	std::string IconResource;

	std::string InitalMapName;
};

class Application
{
public:
	Application() {}
	virtual ~Application() {}

	void Setup(const Configuration &config);
	void Shutdown();

	bool Run();

	std::function<void(Application&)> StartupCallback;
	std::function<void(Application&, double)> PreUpdateCallback;
	std::function<void(Application&, double)> PostUpdateCallback;
	std::function<void(Application&)> ShutdownCallback;

	std::shared_ptr<IsoMap> MapPtr;
	Viewport::Ptr View;

protected: // TODO make a base class so protected makes sense
	void OnStartup();
	void OnPreUpdate();
	void OnPostUpdate();
	void OnShutdown();

	void Draw();

	bool LoadLevel(const std::string& name);

private:
	sf::Time FrameTime;
	sf::Clock Timer;

	void SetupDisplay(const Configuration& config);
};

