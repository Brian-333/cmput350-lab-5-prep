#include <algorithm>
#include <cstdint>
#include <iostream>
#include <memory>
#include <random>
#include <numbers>
#include <vector>

#include <SFML/Graphics.hpp>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 800;
const int FPS_LIMIT = 30;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

void handleInput(sf::Window& window, bool& shouldQuit) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
            shouldQuit = true;
        }

        // ====== ====== ======
        // TODO: (Q2)
        //  implement key presses (1-9) that replace the tween function
        //  with different alternate tween functions.
        //  Functions can be from lecture or from https://easings.net/#
        // ====== ====== ======
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1)) {
            tween = [](float a, float b, float t) {
                // easeInOutSine
                return a + ((b - a) * -(std::cos(std::numbers::pi * t) - 1) / 2);
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2)) {
            tween = [](float a, float b, float t) {
                // easeInQuad
                return a + (b - a) * t * t;
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3)) {
            tween = [](float a, float b, float t) {
                // easeOutQuad
                return b - (1-t)*(1 - t) * (b - a);
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4)) {
            tween = [](float a, float b, float t) {
                // easeOutQuint
                return b - std::pow(1 - t, 5) * (b - a);
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num5)) {
            tween = [](float a, float b, float t) {
                // easeInCirc
                return b - std::sqrt(1 - std::pow(t, 2)) * (b - a);
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num6)) {
            tween = [](float a, float b, float t) {
                // easeOutCirc
                return a + (b - a) * std::sqrt(1 - std::pow(t - 1, 2));
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num7)) {
            tween = [](float a, float b, float t) {
                // easeInBack
                return a + (b - a) * (std::pow(t, 3) * 2.70158 - (std::pow(t, 2) * 1.70158));
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num8)) {
            tween = [](float a, float b, float t) {
                // easeOutBack
                return a + (b - a) * (1.0 + 2.70158 * std::pow(t - 1, 3) + 1.70158 * std::pow(t - 1, 2));
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num9)) {
            tween = [](float a, float b, float t) {
                // easeInOutBack
                return a + (b - a) * (t < 0.5
                ? (std::pow(2 * t, 2) * ((2.70158 + 1) * 2 * t - 2.70158)) / 2
                : (std::pow(2 * t - 2, 2) * ((2.70158 + 1) * (t * 2 - 2) + 2.70158) + 2) / 2);
            };
        } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num0)) {
            tween = [](float a, float b, float t) {
                // Original
                return (1 - t) * a + t * b;
            };
        }
    }
}

void render(sf::RenderWindow& window) {
    // Clear with blue background (sky)
    window.clear(sf::Color::Black);
    // ====== ====== ======
    // TODO: (Q1) Draw circle that moves between
    // the left/right half of the screen.
    // Movement should be governed by the tween function.
    // ====== ====== ======
    static int frame = 0;
    static bool direction = true;
    static int mode = 0;
    static sf::CircleShape circle(10);

    if (direction) {
        frame++;
    } else {
        frame--;
    }
    if (frame == 0 || frame == FPS_LIMIT) {
        direction = !direction;
    }

    circle.setPosition({tween(0, WINDOW_WIDTH, frame / static_cast<float>(FPS_LIMIT)), WINDOW_HEIGHT / 3.0f});
    circle.setFillColor(sf::Color::White);
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======

    window.display();
}

int main() {
    sf::RenderWindow window;

    try {
        // Initialize window
        window.create(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Tween");
        window.setFramerateLimit(FPS_LIMIT);
        // Prevent key repeats.
        window.setKeyRepeatEnabled(false);

        bool shouldQuit = false;
        // Main game loop
        while (window.isOpen()) {
            handleInput(window, shouldQuit);
            if (shouldQuit) {
                break;
            }
            render(window);
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
