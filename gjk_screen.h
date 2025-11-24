#pragma once
#include <SFML/Graphics.hpp>
#include "utils.h"
#include "screens_enum.h"

class GJK
{
	util::Button* back;
	sf::ConvexShape player;
	sf::ConvexShape obstacle;
	sf::Vector2f playerPos = { 0, 0 };
	float playerRotation = 0;

	bool processSimplex(std::vector<sf::Vector2f>& simplex, sf::Vector2f& direction);

public:
	GJK(Screens& screen);

	static sf::Vector2f support(const sf::ConvexShape& shapeA,
		const sf::ConvexShape& shapeB,
		sf::Vector2f direction);

	void handleEvents(const sf::Event& event, const sf::RenderWindow& wnd);

	bool checkCollision();

	void draw(sf::RenderWindow& wnd);
};