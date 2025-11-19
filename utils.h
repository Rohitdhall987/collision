#pragma once
#include <SFML/Graphics.hpp>
#include <functional>

namespace util
{
    class Button
    {
        sf::Vector2f pos;
        sf::RectangleShape box;
        sf::Font font;
        sf::Text* text;
        std::function<void()> callback;

    public:
        Button(sf::Vector2f position, const std::string& title, unsigned int fontSize, sf::Color background);

        void onClick(std::function<void()> cb);

        void draw(sf::RenderWindow& wnd);

        void handleEvent(const sf::Event& event, const sf::RenderWindow& wnd);
    };
}
