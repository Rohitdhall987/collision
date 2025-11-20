#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "screens_enum.h"

class CircleCircle
{
	util::Button* back;
	sf::CircleShape player;
	sf::CircleShape obstacle;
	sf::Vector2f playerPos = { 0, 0 };

public:
	CircleCircle(Screens& screen);

	void handleEvents(const sf::Event& event, const sf::RenderWindow& wnd);

	void checkCollision();

	void draw(sf::RenderWindow& wnd);
};