#pragma once
#include "IsoEngine.h"

class Viewport
{
public:
	typedef std::shared_ptr<Viewport> Ptr;

	Viewport(sf::VideoMode mode, const std::string title, uint32_t style);
	virtual ~Viewport();

	sf::View		View;
	sf::RenderWindow Window;

	void SetView();
	void RestoreView();

	void HandleEvent(sf::Event& sfEvent);

	void SetViewRectTolerance(int x, int y);

	bool TileInView(const sf::Vector2f& pos);
protected:
	bool ViewEnabled = false;
	sf::FloatRect WorldSpaceRect;
	sf::Vector2f WorldSpaceTolerance;
};

