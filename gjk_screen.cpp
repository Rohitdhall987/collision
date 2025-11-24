#include "gjk_screen.h"
#include <cmath>
#include <iostream>

struct EPAResult {
    sf::Vector2f normal;
    float depth;
};


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

float length(const sf::Vector2f& v) {
    return std::sqrt(v.x * v.x + v.y * v.y);
}

sf::Vector2f normalize(const sf::Vector2f& v) {
    float len = length(v);
    return (len == 0) ? sf::Vector2f(0, 0) : sf::Vector2f(v.x / len, v.y / len);
}

sf::Vector2f GJK::support(const sf::ConvexShape& shapeA,
    const sf::ConvexShape& shapeB,
    sf::Vector2f direction)
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

EPAResult EPA(const std::vector<sf::Vector2f>& simplex,
    const sf::ConvexShape& A,
    const sf::ConvexShape& B)
{
    std::vector<sf::Vector2f> polytope = simplex;

    // Ensure counter-clockwise winding order
    float cross = (polytope[1].x - polytope[0].x) * (polytope[2].y - polytope[0].y) -
        (polytope[1].y - polytope[0].y) * (polytope[2].x - polytope[0].x);
    if (cross < 0) {
        std::reverse(polytope.begin(), polytope.end());
    }

    struct Edge { sf::Vector2f normal; float distance; int index; };

    for (int iteration = 0; iteration < 64; iteration++)
    {
        Edge best = { {0,0}, std::numeric_limits<float>::infinity(), -1 };

        // Find closest edge
        for (int i = 0; i < polytope.size(); i++)
        {
            sf::Vector2f a = polytope[i];
            sf::Vector2f b = polytope[(i + 1) % polytope.size()];

            sf::Vector2f edge = b - a;

            // Get perpendicular pointing inward (toward origin)
            sf::Vector2f normal = normalize({ edge.y, -edge.x });

            // Calculate signed distance from origin
            float dist = normal.dot(a);

            // If negative, normal points away from origin, flip it
            if (dist < 0) {
                normal = -normal;
                dist = -dist;
            }

            if (dist < best.distance) {
                best = { normal, dist, i };
            }
        }

        // Get support point in direction of closest edge normal
        sf::Vector2f p = GJK::support(A, B, best.normal);
        float d = best.normal.dot(p);

        // Check if we've found the edge (within tolerance)
        if (d - best.distance < 0.001f)
        {
            return { best.normal, best.distance };
        }

        // Expand polytope
        polytope.insert(polytope.begin() + best.index + 1, p);
    }

    // Fallback if max iterations reached
    std::cout << "EPA: Max iterations reached" << std::endl;
    return { {1,0}, 0 };
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
        sf::Vector2f newPos = playerPos;
        float newRotation = playerRotation;

        if (key->scancode == sf::Keyboard::Scancode::A)
            newPos.x -= speed;
        if (key->scancode == sf::Keyboard::Scancode::D)
            newPos.x += speed;
        if (key->scancode == sf::Keyboard::Scancode::W)
            newPos.y -= speed;
        if (key->scancode == sf::Keyboard::Scancode::S)
            newPos.y += speed;
        if (key->scancode == sf::Keyboard::Scancode::Q)
            newRotation -= 5;
        if (key->scancode == sf::Keyboard::Scancode::E)
            newRotation += 5;

        // Save old state
        sf::Vector2f oldPos = playerPos;
        float oldRotation = playerRotation;

        // Apply new transform
        player.setRotation(sf::degrees(newRotation));
        player.setPosition(newPos);

        // Check collision
        if (checkCollision()) {
            // Collision detected - revert to old position
            player.setPosition(oldPos);
            player.setRotation(sf::degrees(oldRotation));
            playerPos = oldPos;
            playerRotation = oldRotation;
        }
        else {
            // No collision, accept the move
            playerPos = newPos;
            playerRotation = newRotation;
        }
    }
}


bool GJK::checkCollision()
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
            return false;  // No collision

        simplex.push_back(newPoint);

        if (processSimplex(simplex, direction))
        {
            // Collision detected - run EPA to find penetration depth
            EPAResult result = EPA(simplex, player, obstacle);

            // Apply MTV with small buffer to ensure separation
            sf::Vector2f mtv = result.normal * (result.depth + 0.1f);

            sf::Vector2f resolvedPos = playerPos + mtv;
            player.setPosition(resolvedPos);

            std::cout << "Collision resolved with MTV: "
                << mtv.x << ", " << mtv.y
                << " (depth: " << result.depth << ")" << std::endl;

            return true;  // Collision was detected and resolved
        }
    }
}


void GJK::draw(sf::RenderWindow& wnd)
{

    wnd.draw(obstacle);
    wnd.draw(player);

    back->draw(wnd);
}