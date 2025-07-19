
#pragma once

#include <SFML/Graphics.hpp>
#include "data.hpp"
#include <vector>
#include <memory>
#include <cmath>
#include <map>

struct Node {

    int depth;
    float split;
    float result;
    bool is_leaf;
    bool first_column;
    std::unique_ptr<Node> lower;
    std::unique_ptr<Node> upper;

     Node(
        int depth_val,
        float split_val,
        float result_val,
        bool is_leaf_val,
        bool first_column_val
    ) {
        depth = depth_val;
        split = split_val;
        result = result_val;
        is_leaf = is_leaf_val;
        first_column = first_column_val;
    }
};

struct Simulation {

    Data data;
    std::unique_ptr<Node> tree;

    static float sum(std::vector<float> x) {

        float sum = 0;
        
        for (float i : x) {
            sum += i;
        }

        return sum;
    }

    static float mean(std::vector<float> x) {
        return sum(x) / float(x.size());
    }

    static float sd(std::vector<float> x) {

        float x_mean = mean(x);

        for (float& i : x) {
            i = pow(i - x_mean, 2);
        }

        return sqrt(mean(x));
    }

    static void best_split(
        Data data,
        float& final_sd,
        float& final_split,
        bool first_column
    ) {

        bool first_iter = true;
        
        for (auto& split_row : data.contents) {

            float sd_diff;
            float split = split_row.y;

            if (first_column) {
                split = split_row.x;
            }
            
            std::vector<float> lower;
            std::vector<float> upper;

            for (auto& row : data.contents) {

                float value = row.y;

                if (first_column) {
                    value = row.x;
                }

                if (value < split) {
                    lower.push_back(row.group);
                } else {
                    upper.push_back(row.group);
                }
            }

            if (lower.size() == 0 | upper.size() == 0) {
                continue;
            }

            sd_diff = abs(sd(lower) - sd(upper));

            if (first_iter | (sd_diff > final_sd)) {
                final_sd = sd_diff;
                final_split = split;
                first_iter = false;
            }
        }
    }

    static void divide(
        Data data,
        Data& lower,
        Data& upper,
        float split,
        bool first_column
    ) {

        for (auto& row : data.contents) {
            
            if (first_column & row.x < split) {
                lower.contents.push_back(row);
            }

            if (!first_column & row.y < split) {
                lower.contents.push_back(row);
            }

            if (first_column & row.x >= split) {
                upper.contents.push_back(row);
            }

            if (!first_column & row.y >= split) {
                upper.contents.push_back(row);
            }
        }
    }

    static std::unique_ptr<Node> make_node(
        int depth,
        float split,
        float result,
        bool is_leaf,
        bool first_column
    ) {
        return std::make_unique<Node>(
            depth,
            split,
            result,
            is_leaf,
            first_column
        );
    }

    static std::unique_ptr<Node> learn(Data local_data, int depth) {

        std::vector<float> result_vec;

        for (auto& row : local_data.contents) {
            result_vec.push_back(row.group);
        }

        float result = mean(result_vec);

        if (local_data.contents.size() < 5) {
            auto node_final = make_node(depth, 0.0, result, true, false);
            return node_final;
        }

        float first_sd;
        float first_split;
        
        float second_sd;
        float second_split;

        best_split(local_data, first_sd, first_split, true);
        best_split(local_data, second_sd, second_split, false);

        bool first_column = true;
        float split = first_split;
        
        if (first_sd < second_sd) {
            first_column = false;
            split = second_split;
        }
        
        Data lower;
        Data upper;
    
        divide(local_data, lower, upper, split, first_column);

        auto node = make_node(
            depth,
            split,
            result,
            false,
            first_column
        );

        node->lower = learn(lower, depth + 1);
        node->upper = learn(upper, depth + 1);

        return node;
    }

    static float predict(Point x, const std::unique_ptr<Node>& tree) {
        
        if (!tree) {
            return 0.0f;
        }
    
        if (tree->is_leaf) {
            return tree->result;
        }
    
        if (tree->first_column) {
            if (x.x < tree->split) {
                return predict(x, tree->lower);
            } else {
                return predict(x, tree->upper);
            }
        } else {
            if (x.y < tree->split) {
                return predict(x, tree->lower);
            } else {
                return predict(x, tree->upper);
            }
        }
    }

    Simulation(int x, int y) {
        data.load(x, y);
        tree = learn(data, 1);
    }

    void render(sf::RenderWindow& window) {
        
        window.clear();

        sf::CircleShape circle;
        circle.setPointCount(32);
        circle.setRadius(5);
        circle.setOrigin({1, 1});

        for (float i = 0; i < 720; i++) {
            for (float j = 0; j < 720; j++) {
                float pred = round(predict({i, j}, tree));
                circle.setPosition({i, j});
                if (pred < 0.5) {   
                    circle.setFillColor(sf::Color::Red);
                } else {
                    circle.setFillColor(sf::Color::Blue);
                }
                window.draw(circle);
            }
        }
    
        for (auto& i : data.contents) {
            circle.setPosition({i.x, i.y});
            circle.setFillColor(i.color);
            window.draw(circle);
        }
        
        window.display(); 
    }

    void advance(sf::RenderWindow& window) {
        render(window);
    };
};
