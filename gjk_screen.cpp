#include "gjk_screen.h"
#include <cmath>
#include <iostream>

sf::Vector2f tripleProduct(sf::Vector2f a, sf::Vector2f b, sf::Vector2f c)
{
    float ac = a.dot(c);
    float bc = b.dot(c);
    return sf::Vector2f(
        b.x * ac - a.x * bc,
        b.y * ac - a.y * bc
    );
}

sf::Vector2f findFurthestPoint(const sf::ConvexShape& shape, sf::Vector2f dir)
{
    float furthest = -std::numeric_limits<float>::infinity();
    sf::Vector2f best;

    unsigned int count = shape.getPointCount();
    for (unsigned int i = 0; i < count; ++i)
    {
        sf::Vector2f world = shape.getTransform().transformPoint(shape.getPoint(i));
        float projection = world.dot(dir);

        if (projection > furthest)
        {
            furthest = projection;
            best = world;
        }
    }

    return best;
}

sf::Vector2f GJK::support(sf::ConvexShape shapeA, sf::ConvexShape shapeB, sf::Vector2f direction)
{
    sf::Vector2f pA = findFurthestPoint(shapeA, direction);
    sf::Vector2f pB = findFurthestPoint(shapeB, -direction);

    return pA - pB;
}


bool GJK::processSimplex(std::vector<sf::Vector2f>& simplex, sf::Vector2f& direction)
{
    sf::Vector2f A = simplex.back();
    sf::Vector2f AO = -A;

    if (simplex.size() == 2)
    {
        sf::Vector2f B = simplex[0];
        sf::Vector2f AB = B - A;

        if (AB.dot(AO) > 0)
        {
            direction = tripleProduct(AB, AO, AB);
        }
        else
        {
            simplex = { A };
            direction = AO;
        }

        return false;
    }

    
    sf::Vector2f B = simplex[1];
    sf::Vector2f C = simplex[0];

    sf::Vector2f AB = B - A;
    sf::Vector2f AC = C - A;

    sf::Vector2f ABperp = tripleProduct(AC, AB, AB);
    sf::Vector2f ACperp = tripleProduct(AB, AC, AC);

    if (ABperp.dot(AO) > 0)
    {
        simplex = { A, B };
        direction = ABperp;
        return false;
    }

    if (ACperp.dot(AO) > 0)
    {
        simplex = { A, C };
        direction = ACperp;
        return false;
    }

    return true;
}



GJK::GJK(Screens& screen)
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

void GJK::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
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


void GJK::checkCollision()
{
    std::vector<sf::Vector2f> simplex;

    sf::Vector2f direction = { 1.0f, 0.0f };

    sf::Vector2f point = support(player, obstacle, direction);
    simplex.push_back(point);

    direction = -point;

    while (true)
    {
        sf::Vector2f newPoint = support(player, obstacle, direction);

        if (newPoint.dot(direction) <= 0)
            return;

        simplex.push_back(newPoint);

        if (processSimplex(simplex, direction)) 
        {
            // resolve collission
            std::cout << "collission!" << std::endl;
            return;
        }
    }
}

void GJK::draw(sf::RenderWindow& wnd)
{

    wnd.draw(obstacle);
    wnd.draw(player);

    back->draw(wnd);
}