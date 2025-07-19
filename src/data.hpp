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
    float group = 0;
    sf::Color color = sf::Color::White;
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

    void load(
        float display_x,
        float display_y,
        float buffer = 50,
        std::string path = "../data/data.csv"
    ) {

        if (contents.size() > 0) {
            std::cout << "Warning: load() cancelled because data exists!\n";
            return;
        }

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
                    if (result == 0) {
                        point.group = result;
                        point.color = {21, 126, 192};
                    } else if (result == 1) {
                        point.group = result;
                        point.color = {245, 145, 48};
                    } else {
                        std::cout << "The third column must be 0 or 1!\n";
                        std::exit(1);
                    }
                }
                
                position += 1;
            }

            if (position != 3) {
                std::cout << "Each row must contain 3 values!\n";
                std::cout << "Line " << index << " is malformed\n";
                std::exit(1);
            }

            if (index > 1000) {
                std::cout << "Warning: only the first 1000 points are used\n";
                break;
            }

            contents.push_back(point);
            index += 1;
        }

        file.close();
        
        for (auto& i : contents) {
            i.x = (i.x * (display_x - buffer * 2) + buffer);
            i.y = (i.y * (display_y - buffer * 2) + buffer);
        }
    }
};
