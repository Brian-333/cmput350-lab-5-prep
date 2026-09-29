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
const int ANIMATION_TIME = 2;

// global tween function
std::function<float(float, float, float)> tween = [](float a, float b, float t) {
    return (1 - t) * a + t * b;
};

std::function<sf::Vector2f(int)> graph_point = [](int frame) {
    return sf::Vector2f(
        (WINDOW_WIDTH / 3.0f) + static_cast<float>(frame) / (FPS_LIMIT * ANIMATION_TIME) * (WINDOW_WIDTH / 3.0f), 
        tween(WINDOW_HEIGHT - WINDOW_HEIGHT / 6.0f, WINDOW_HEIGHT * 2 / 3.0f, frame / static_cast<float>(FPS_LIMIT * ANIMATION_TIME))
    );
};

std::function<void(sf::VertexArray&)> graph = [](sf::VertexArray& g) {
    for (int i = 0; i < FPS_LIMIT * ANIMATION_TIME; i++) {
        g[i].position = graph_point(i);
        g[i].color = sf::Color::Blue;
    }
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
    // Use static variables to keep track of the frame and direction
    static int frame = 0;
    static bool direction = true;
    static sf::CircleShape circle(10);
    circle.setOrigin({circle.getRadius(), circle.getRadius()});

    if (direction) {
        frame++;
    } else {
        frame--;
    }
    if (frame == 0 || frame == (FPS_LIMIT * ANIMATION_TIME)) {
        // Reset frame to 0 or change direction
        // direction = !direction;
        frame = 0;
    }

    circle.setPosition({tween(0, WINDOW_WIDTH, frame / static_cast<float>(FPS_LIMIT * ANIMATION_TIME)), WINDOW_HEIGHT / 3.0f});
    circle.setFillColor(sf::Color::White);
    window.draw(circle);

    // ====== ====== ======
    // TODO: (Q3) Draw tween function graph with a dot
    // on the current portion of the curve
    // ====== ====== ======
    static sf::VertexArray graph_line(sf::PrimitiveType::LineStrip, FPS_LIMIT * ANIMATION_TIME);
    graph(graph_line);
    window.draw(graph_line);
    
    static sf::CircleShape dot(5);
    dot.setPosition(graph_point(frame));
    dot.setFillColor(sf::Color::Red);
    dot.setOrigin({dot.getRadius(), dot.getRadius()});
    window.draw(dot);

    static sf::Vector2f graph_origin = {WINDOW_WIDTH / 3.0f, WINDOW_HEIGHT - WINDOW_HEIGHT / 6.0f};
    // sf::Vector2f(
    //     (WINDOW_WIDTH / 3.0f) + static_cast<float>(frame) / (FPS_LIMIT * ANIMATION_TIME) * (WINDOW_WIDTH / 3.0f), 
    //     min: WINDOW_WIDTH / 3.0f,
    //     max: WINDOW_WIDTH / 3.0f + (WINDOW_WIDTH / 3.0f),
    //     tween(WINDOW_HEIGHT - WINDOW_HEIGHT / 6.0f, WINDOW_HEIGHT * 2 / 3.0f, frame / static_cast<float>(FPS_LIMIT * ANIMATION_TIME))
    //     min: WINDOW_HEIGHT - WINDOW_HEIGHT / 6.0f,
    //     max: WINDOW_HEIGHT * 2 / 3.0f,
    // );
    static sf::Vector2f graph_end_height = {WINDOW_WIDTH / 3.0f, WINDOW_HEIGHT * 2 / 3.0f};
    static sf::Vector2f graph_end_width = {WINDOW_WIDTH * 2 / 3.0f, WINDOW_HEIGHT - WINDOW_HEIGHT / 6.0f};
    // Vertical line
    static sf::Vertex line1[] = {
        sf::Vertex(graph_origin),
        sf::Vertex(graph_end_height)
    };
    // Horizontal line
    static sf::Vertex line2[] = {
        sf::Vertex(graph_origin),
        sf::Vertex(graph_end_width)
    };
    
    window.draw(line1, 2, sf::PrimitiveType::Lines);
    window.draw(line2, 2, sf::PrimitiveType::Lines);

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
