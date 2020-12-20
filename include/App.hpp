#pragma once
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SFML/Graphics.hpp>
#include <random>
#include "Camera.hpp"
#include "Generators.hpp"
#include "LoadShader.hpp"

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

    std::random_device rand;
    
    bool animation;
    sf::Clock animationClock;
    float cubeScaleFactor;
    
    GLuint VAO;
    GLuint VBO;
    GLuint verticesBuffer;
    GLuint colorBuffer;
    GLuint indicesBuffer;

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> colors;
    std::vector<GLuint> indices;

    GLuint shaderID;
    
    glm::mat4 projectionMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionView;

    GLuint mvpID;
    GLuint viewID;
    GLuint modelID;

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
