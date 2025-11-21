#include "sat_screen.h"
#include <cmath>
#include <iostream>

std::vector<sf::Vector2f> SAT::calcNormals(sf::ConvexShape shape)
{
    std::vector<sf::Vector2f> norms;
    unsigned int len = shape.getPointCount();

    for (int i = 0; i < len; ++i)
    {
        sf::Vector2f a = shape.getPoint(i);
        sf::Vector2f b;

        if (i + 1 == len)
            b = shape.getPoint(0);
        else
            b = shape.getPoint(i + 1);

        sf::Vector2f edge = b - a;
        sf::Vector2f perpendicula = { edge.y, -edge.x };

        float length = sqrt(pow(perpendicula.x, 2) + pow(perpendicula.y, 2));

        sf::Vector2f normal = perpendicula / length;

        norms.push_back(normal);
    }

    return norms;
}

sf::Vector2f SAT::calcBounds(sf::ConvexShape shape, sf::Vector2f axis)
{
    bool first = true;
    sf::Vector2f bounds;
    unsigned int len = shape.getPointCount();
    for (int i = 0; i < len; ++i)
    {
        sf::Vector2f worldCord = shape.getTransform().transformPoint(shape.getPoint(i));
        float projection = worldCord.x * axis.x + worldCord.y * axis.y;

        if (first) 
        {
            bounds = { projection,projection };
            first = false;
        }
        else 
        {
            bounds.x = std::min(bounds.x, projection);
            bounds.y = std::max(bounds.y, projection);
        }
    }

    return bounds;
}

SAT::SAT(Screens& screen)
{
    back = new util::Button({ 50, 25 }, "BACK", 24, sf::Color::Red);

    back->onClick([&screen] {
        screen = Screens::MainMenu;
        });

    player.setPointCount(3);
    player.setPoint(0, sf::Vector2f(0.f, 0.f));
    player.setPoint(1, sf::Vector2f(120.f, 30.f));
    player.setPoint(2, sf::Vector2f(40.f, 140.f));

    sf::FloatRect pBounds = player.getLocalBounds();
    player.setOrigin({ pBounds.size.x / 2.f, pBounds.size.y / 2.f });

    obstacle.setPointCount(5);
    obstacle.setPoint(0, sf::Vector2f(60.f, 0.f));
    obstacle.setPoint(1, sf::Vector2f(120.f, 40.f));
    obstacle.setPoint(2, sf::Vector2f(100.f, 110.f));
    obstacle.setPoint(3, sf::Vector2f(20.f, 110.f));
    obstacle.setPoint(4, sf::Vector2f(0.f, 40.f));
    obstacle.setFillColor(sf::Color(60, 140, 200));

    sf::FloatRect bounds = obstacle.getLocalBounds();
    obstacle.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });

    obstacle.setPosition({ 450,450 });
}

void SAT::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
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
        if (key->scancode == sf::Keyboard::Scancode::Q)
            playerRotation -= 5;
        if (key->scancode == sf::Keyboard::Scancode::E)
            playerRotation += 5;

        player.setRotation(sf::degrees(playerRotation));
        player.setPosition(playerPos);
        checkCollision();
        player.setPosition(playerPos);
    }
}


void SAT::checkCollision()
{
    std::vector<sf::Vector2f> normals;

    for (sf::Vector2f normal : calcNormals(player)) 
    {
        normals.push_back(normal);
    }

    for (sf::Vector2f normal : calcNormals(obstacle)) 
    {
        normals.push_back(normal);
    }

    for (int i = 0; i < normals.size(); ++i)
    {
        sf::Vector2f playerBound = calcBounds(player, normals[i]);
        sf::Vector2f obstacleBound = calcBounds(obstacle, normals[i]);
        float minA = playerBound.x, minB = obstacleBound.x, maxA = playerBound.y, maxB = obstacleBound.y;

        if (maxA < minB || maxB < minA)
        {
            return; // found gap, exit
        }
    }

    std::cout << "collission!!" << std::endl;
    // solve collission
}

void SAT::draw(sf::RenderWindow& wnd)
{

    wnd.draw(obstacle);
    wnd.draw(player);

    back->draw(wnd);
}