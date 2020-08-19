#include "Viewport.h"

Viewport::Viewport(sf::VideoMode mode, const std::string title, uint32_t style) : Window(mode,title,style)
{
	View.setCenter(0, 0);
	View.setSize((float)Window.getSize().x, (float)Window.getSize().y);

	SetView();
}

Viewport::~Viewport()
{
	if (Window.isOpen())
	{
		Window.setView(Window.getDefaultView());
		Window.close();
	}
}

void Viewport::SetView()
{
	Window.setView(View);
	ViewEnabled = true;

	auto zero = Window.mapPixelToCoords(sf::Vector2i(0, 0));
	auto edge = Window.mapPixelToCoords(sf::Vector2i(Window.getSize().x, Window.getSize().y));

	WorldSpaceRect = sf::FloatRect(zero.x, zero.y, edge.x - zero.x, edge.y - zero.y);
	WorldSpaceRect.left -= WorldSpaceTolerance.x * 1;
	WorldSpaceRect.width += WorldSpaceTolerance.x * 2;
	WorldSpaceRect.top -= WorldSpaceTolerance.y * 2;
	WorldSpaceRect.height += WorldSpaceTolerance.y * 4;
}

void Viewport::RestoreView()
{
	Window.setView(Window.getDefaultView());
	ViewEnabled = false;
}

void Viewport::HandleEvent(sf::Event& sfEvent)
{
	if (sfEvent.type == sf::Event::Resized)
	{
		View.setSize((float)sfEvent.size.width, (float)sfEvent.size.height);

		if (ViewEnabled)
			SetView();
	}
}

void Viewport::SetViewRectTolerance(int x, int y)
{
	WorldSpaceTolerance = sf::Vector2f((float)x, (float)y);
	if (ViewEnabled)
		SetView();
}

bool Viewport::TileInView(const sf::Vector2f& pos)
{
	return WorldSpaceRect.contains(pos);
}

