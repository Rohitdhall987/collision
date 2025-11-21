#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "screens_enum.h"

class SAT
{
	util::Button* back;
	sf::ConvexShape player;
	sf::ConvexShape obstacle;
	sf::Vector2f playerPos = { 0, 0 };
	float playerRotation = 0;

	std::vector<sf::Vector2f> calcNormals(sf::ConvexShape shape);
	sf::Vector2f calcBounds(sf::ConvexShape shape, sf::Vector2f axis);

public:
	SAT(Screens& screen);

	void handleEvents(const sf::Event& event, const sf::RenderWindow& wnd);

	void checkCollision();

	void draw(sf::RenderWindow& wnd);
};