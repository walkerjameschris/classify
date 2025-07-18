#pragma once

#include <SFML/Graphics.hpp>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

struct Point {
    float x;
    float y;
    float group;
    sf::Color color = sf::Color::Blue;
};

struct Data {

    std::vector<Point> contents;

    static float str_to_num(std::string x, int index) {

        try {
            return std::stof(x);
        } catch (...) {
            std::cout << "File is malformed on line ";
            std::cout << std::to_string(index) + "\n";
            std::cout << "Cannot convert value to number!\n";
            std::exit(1);
        }
    }

    Data(std::string path = "../data/data.csv") {

        if (!std::filesystem::is_regular_file(path)) {
            std::cout << path + " is not a known regular file!\n";
            std::exit(1);
        }

        std::ifstream file(path);
        std::string line;
        int index = 0;

        while (std::getline(file, line)) {

            std::stringstream stream(line);
            std::string value;
            Point point;

            int position = 0;

            while (std::getline(stream, value, ',')) {
                
                float result = str_to_num(value, index);
                
                if (position == 0) {
                    point.x = result;
                }
                
                if (position == 1) {
                    point.y = result;
                }
                
                if (position == 2) {
                    point.group = result;
                    if (result > 0.5) {
                        point.color = sf::Color::Red;
                    }
                }
                
                position += 1;
            }

            contents.push_back(point);
            index += 1;
        }

        file.close();
    }

    void adjust(float display_x, float display_y, float buffer = 50) {
        for (auto& i : contents) {
            i.x = (i.x * (display_x - buffer * 2) + buffer);
            i.y = (i.y * (display_y - buffer * 2) + buffer);
        }
    }
};
