#include "utils.h"
#include <iostream>
#include <filesystem>

using namespace util;


Button::Button(sf::Vector2f position, const std::string& title, unsigned int fontSize, sf::Color background)
{

    if (!font.openFromFile(std::filesystem::path("assets/JetBrainsMono-Bold.ttf")))
        std::cerr << "unable to load font file\n";

    pos = position;


    text = new sf::Text(font, title, fontSize);
    text->setFillColor(sf::Color::White);


    sf::FloatRect bounds = text->getLocalBounds();


    float paddingX = 15.f;
    float paddingY = 10.f;

    box.setSize({
        bounds.size.x + paddingX * 2,
        bounds.size.y + paddingY * 2
        });

    box.setFillColor(background);


    box.setPosition({ pos.x - box.getSize().x / 2, pos.y - box.getSize().y / 2 });


    text->setPosition({
        pos.x - box.getSize().x / 2 + paddingX - bounds.position.x,
        pos.y - box.getSize().y / 2 + paddingY - bounds.position.y
        });
}

void Button::onClick(std::function<void()> cb)
{
    callback = cb;
}


void Button::handleEvent(const sf::Event& event, const sf::RenderWindow& wnd)
{
    if (event.is<sf::Event::MouseButtonPressed>())
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(wnd);
        sf::Vector2f mousePos(static_cast<float>(pixelPos.x),
            static_cast<float>(pixelPos.y));

        if (box.getGlobalBounds().contains(mousePos))
        {
            if (callback)
                callback();
        }
    }
}


void Button::draw(sf::RenderWindow& wnd)
{
    wnd.draw(box);
    wnd.draw(*text);
}
