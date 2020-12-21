#pragma once

#include "Camera.hpp"
#include "LoadShader.hpp"
#include "Random.hpp"
#include "Cube.hpp"
#include "RandomObject.hpp"

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include <memory>
#include <iostream>


class App
{
public:
    App();
    ~App() = default;

    void run();
private:
    void mainLoop();

    void handleEvents();
    void logic(const float&);
    void draw();

    void handleResize();
private:
    std::string titlePrefix;
    sf::RenderWindow window;
    sf::ContextSettings contextSettings;
    std::map<int, bool> keyMap;
    
    // all about animation
    bool animation;
    sf::Clock animationClock;
    float cubeScaleFactor;
    
    std::vector<std::unique_ptr<Object>> objects;

    GLuint shaderID;
    
    // MVP matrix
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionView;

    Camera camera;

    sf::Clock startClock;

    // Performance data
    sf::Clock fpsClock;
    unsigned fpsCount = 0;
    unsigned actualTickCount = 0;
    const unsigned tickCount = 120;

    template<typename T = float>
    inline T getAspectRatio() {
      return static_cast<T>(window.getSize().x) / window.getSize().y;
    }

    void showDebugInfo();
};
