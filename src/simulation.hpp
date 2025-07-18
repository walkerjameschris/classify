
#pragma once

#include <SFML/Graphics.hpp>
#include "data.hpp"
#include <vector>
#include <cmath>
#include <map>

struct Simulation {

    Data data;
    sf::CircleShape circle;
    
    float display_x;
    float display_y;

    Simulation(int x, int y) {
        data.adjust(x, y);
        display_x = x;
        display_y = y;
    }

    void advance(sf::RenderWindow& window) {

        window.clear();
    
        for (auto& i : data.contents) {
            circle.setPointCount(32);
            circle.setRadius(5);
            circle.setPosition({i.x, i.y});
            circle.setFillColor(i.color);
            circle.setOrigin({1, 1});
            window.draw(circle);
        }
    
        window.display();    
    };
};
