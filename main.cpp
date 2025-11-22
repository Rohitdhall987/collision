#include <SFML/Graphics.hpp>
#include <iostream>
#include "screens_enum.h"
#include "main_menu.h"
#include "aabb_screen.h"
#include "circle_screen.h"
#include "sat_screen.h"
#include "gjk_screen.h"

int main()
{
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;

    sf::RenderWindow window(
        sf::VideoMode({ 900u, 900u }),
        "Collision",
        sf::State::Windowed,
        settings
    );

    window.setVerticalSyncEnabled(1);

    Screens screen = Screens::MainMenu;

    MainMenu mainScreen(screen);
    AABB aabbScreen(screen);
    CircleCircle circleScreen(screen);
    SAT satScreen(screen);
    GJK gjkScreen(screen);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            if(screen == Screens::MainMenu)
                mainScreen.handleEvents(*event, window);
            if (screen == Screens::AABB)
                aabbScreen.handleEvents(*event, window);
            if (screen == Screens::CircleCircle)
                circleScreen.handleEvents(*event, window);
            if (screen == Screens::SAT)
                satScreen.handleEvents(*event, window);
            if (screen == Screens::GJK)
                gjkScreen.handleEvents(*event, window);
        }

        window.clear();

        switch (screen) {
        case Screens::MainMenu:
            mainScreen.draw(window);
            break;

        case Screens::AABB:
            aabbScreen.checkCollision();
            aabbScreen.draw(window);
            break;
        case Screens::CircleCircle:
            circleScreen.checkCollision();
            circleScreen.draw(window);
            break;

        case Screens::SAT:
            satScreen.checkCollision();
            satScreen.draw(window);
            break;

        case Screens::GJK:
            gjkScreen.checkCollision();
            gjkScreen.draw(window);
            break;
            
        default:
            break;
        }

        window.display();
    }
}
