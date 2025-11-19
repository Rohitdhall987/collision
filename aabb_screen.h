#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "screens_enum.h"

class AABB
{
	util::Button* back;
	sf::RectangleShape player;
	sf::RectangleShape obstacle;
	sf::Vector2f playerPos = { 0, 0};

public:
	AABB(Screens& screen);

	void handleEvents(const sf::Event& event, const sf::RenderWindow& wnd);

	void checkCollision();

	void draw(sf::RenderWindow& wnd);
};