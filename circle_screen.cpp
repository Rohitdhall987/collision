#include "circle_screen.h"

CircleCircle::CircleCircle(Screens& screen)
{
    back = new util::Button({ 50, 25 }, "BACK", 24, sf::Color::Red);

    back->onClick([&screen] {
        screen = Screens::MainMenu;
        });

    player.setRadius(80);
    player.setOrigin({ 80, 80 });
    player.setFillColor(sf::Color::Green);

    obstacle.setPosition({ 450, 450 });
    obstacle.setRadius(120);
    obstacle.setOrigin({ 120, 120 });
    obstacle.setFillColor(sf::Color::Blue);
}

void CircleCircle::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
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

        checkCollision();
        player.setPosition(playerPos);
    }
}


void CircleCircle::checkCollision()
{
    sf::Vector2f c1 = playerPos;
    sf::Vector2f c2 = obstacle.getPosition();

    float r1 = player.getRadius();
    float r2 = obstacle.getRadius();

    float dis = (c2.x - c1.x) * (c2.x - c1.x) + (c2.y - c1.y) * (c2.y - c1.y);
    float radii = (r1 + r2) * (r1 + r2);
    if (dis <= radii) {
        float distance = std::sqrt(dis);
        float minDist = r1 + r2;
        float overlap = minDist - distance;

        sf::Vector2f diff = c1 - c2;

        if (distance != 0)
        {
            sf::Vector2f norm = diff / distance;
            playerPos = c1 + norm * overlap;
        }
    }
}

void CircleCircle::draw(sf::RenderWindow& wnd)
{

    wnd.draw(obstacle);
    wnd.draw(player);

    back->draw(wnd);
}