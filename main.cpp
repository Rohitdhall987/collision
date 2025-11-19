#include <SFML/Graphics.hpp>
#include <iostream>
#include "utils.h"
#include "screens_enum.h"
#include "main_menu.h"
#include "aabb_screen.h"

int main()
{
    sf::RenderWindow window(
        sf::VideoMode({ 900u, 900u }),
        "Collision"
    );

    window.setVerticalSyncEnabled(1);

    Screens screen = Screens::MainMenu;

    MainMenu mainScreen(screen);
    AABB aabbScreen(screen);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();

            mainScreen.handleEvents(*event, window);
            aabbScreen.handleEvents(*event, window);
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

        default:
            break;
        }

        window.display();
    }
}
