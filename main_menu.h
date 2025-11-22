#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "screens_enum.h"

class MainMenu
{
	util::Button* btnAABB;
	util::Button* btnCircle;
	util::Button* btnSAT;
	util::Button* btnGJK;
	Screens screen = Screens::MainMenu;

public:
	MainMenu(Screens& screen);

	void handleEvents(const sf::Event& event, const sf::RenderWindow& wnd);

	void draw(sf::RenderWindow& wnd);
};