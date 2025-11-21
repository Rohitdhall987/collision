#include <iostream>
#include "main_menu.h"


MainMenu::MainMenu(Screens& screen)
{
    btnAABB = new util::Button({ 450, 150 }, "AABB", 32, sf::Color(50, 50, 200));

    btnAABB->onClick([&screen] {
        screen = Screens::AABB;
        });

    btnCircle = new util::Button({ 450, 250 }, "CIRCLE - CIRCLE", 32, sf::Color(50, 100, 75));

    btnCircle->onClick([&screen] {
        screen = Screens::CircleCircle;
        });

    btnSAT = new util::Button({ 450, 350 }, "SAT", 32, sf::Color(100, 120, 65));

    btnSAT->onClick([&screen] {
        screen = Screens::SAT;
        });
}

void MainMenu::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
{
    btnAABB->handleEvent(event, wnd);
    btnCircle->handleEvent(event, wnd);
    btnSAT->handleEvent(event, wnd);
}

void MainMenu::draw(sf::RenderWindow& wnd)
{
    btnAABB->draw(wnd);
    btnCircle->draw(wnd);
    btnSAT->draw(wnd);
}
