#include "aabb_screen.h"

AABB::AABB(Screens& screen)
{
	back = new util::Button({ 50, 25}, "BACK", 24, sf::Color::Red);

	back->onClick([&screen] {
		screen = Screens::MainMenu;
		});

	player.setSize({ 80, 80 });
	player.setFillColor(sf::Color::Green);

	obstacle.setPosition({ 300, 300 });
	obstacle.setSize({ 120, 120 });
	obstacle.setFillColor(sf::Color::Blue);
}

void AABB::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
{
    back->handleEvent(event, wnd);

    if (auto key = event.getIf<sf::Event::KeyPressed>()) {
        float speed = 20.0f;

        if (key->scancode == sf::Keyboard::Scancode::A)
            playerPos.x -= speed;
        if (key->scancode == sf::Keyboard::Scancode::D)
            playerPos.x += speed;
        if (key->scancode == sf::Keyboard::Scancode::W)
            playerPos.y -= speed;
        if (key->scancode == sf::Keyboard::Scancode::S)
            playerPos.y += speed;

        player.setPosition(playerPos);
        checkCollision();
        player.setPosition(playerPos); 
    }
}


void AABB::checkCollision()
{
    sf::FloatRect p = player.getGlobalBounds();
    sf::FloatRect o = obstacle.getGlobalBounds();

    if (p.findIntersection(o))
    {

        float overlapLeft = (p.position.x + p.size.x) - o.position.x;
        float overlapRight = (o.position.x + o.size.x) - p.position.x;
        float overlapTop = (p.position.y + p.size.y) - o.position.y;
        float overlapBottom = (o.position.y + o.size.y) - p.position.y;


        float minX = std::min(overlapLeft, overlapRight);
        float minY = std::min(overlapTop, overlapBottom);


        if (minX < minY)
        {
            if (overlapLeft < overlapRight)
                playerPos.x -= overlapLeft;
            else
                playerPos.x += overlapRight;
        }
        else
        {
            if (overlapTop < overlapBottom)
                playerPos.y -= overlapTop;
            else
                playerPos.y += overlapBottom;
        }
    }
}

void AABB::draw(sf::RenderWindow& wnd) 
{

	wnd.draw(obstacle);
	wnd.draw(player);

	back->draw(wnd);
}