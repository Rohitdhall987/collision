#include <iostream>
#include "main_menu.h"


MainMenu::MainMenu(Screens& screen)
{
    btnAABB = new util::Button({ 450, 150 }, "AABB", 32, sf::Color(50, 50, 200));

    btnAABB->onClick([&screen] {
        screen = Screens::AABB;
        });
}

void MainMenu::handleEvents(const sf::Event& event, const sf::RenderWindow& wnd)
{
    btnAABB->handleEvent(event, wnd);
}

void MainMenu::draw(sf::RenderWindow& wnd)
{
    btnAABB->draw(wnd);
}
