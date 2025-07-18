#include <SFML/Graphics.hpp>
#include "src/simulation.hpp"

int main() {

    const int frame_rate = 60;
    const int display_x = 720;
    const int display_y = 720;
    
    Simulation simulation(display_x, display_y);
    
    sf::VideoMode window_scale(display_x, display_y);
    sf::RenderWindow window(window_scale, "Classify");
    sf::Clock clock;
    sf::Event event;

    window.setFramerateLimit(frame_rate);

    while (window.isOpen()) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        simulation.advance(window);
    }

    return 0;
}