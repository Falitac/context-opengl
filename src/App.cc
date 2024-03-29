#include "../include/App.hpp"

App::App()
  : contextSettings(24, 8, 4, 4, 6)
  , titlePrefix("OpenGL ")
  , camera(glm::vec3(0.f, 0.f, -7.f))
{
}


void App::run() {
  window.create(
    {1600, 900},
    titlePrefix,
    sf::Style::Default,
    contextSettings
  );

  window.setActive(true);

  glewExperimental = true;
  if(glewInit() != GLEW_OK) {
    std::cout << "GLEW init failure" << std::endl;
  }

  glEnable(GL_DEPTH_BUFFER);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glClearColor(0.1f, 0.1f, 0.2f, 1.0f);

  shaderID = LoadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
  getShaderUniforms(shaderID);

  vertices = {
    -1.0f, -1.0f, 0.0f,
    +1.0f, +1.0f, 0.0f,
    -1.0f, +1.0f, 0.0f,

    -1.0f, -1.0f, 0.0f,
    +1.0f, -1.0f, 0.0f,
    +1.0f, +1.0f, 0.0f,
  };
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  std::cout << vertices.size() * sizeof(float) << "\n";
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3* sizeof(float), nullptr);
  glEnableVertexAttribArray(0);

  mainLoop();
  glDeleteProgram(shaderID);
}

void App::handleEvents() {
  for(sf::Event event; window.pollEvent(event);) {
    switch(event.type) {
      case sf::Event::Closed:
        window.close();
      break;
      case sf::Event::KeyPressed:
        keyMap[event.key.code] = true;
        if(event.key.control && keyMap[sf::Keyboard::W]) {
          window.close();
        }
      break;
      case sf::Event::KeyReleased:
        keyMap[event.key.code] = false;
      break;
      case sf::Event::Resized:
        handleResize();
      break;
      case sf::Event::MouseWheelScrolled: {
        auto delta = event.mouseWheelScroll.delta;
        zoom *= 1.0 + delta / 20.f;

      }
      break;
      case sf::Event::MouseButtonPressed: {
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        }
      }
      break;
      default:
      break;
    }
  }

}

void App::logic(const float& deltaTime) {
  auto camSpeed = 0.f;
  auto strafeSpeed = 0.f;
  auto rotateHor = 0.0f;
  auto rotateVert = 0.0f;
  
  auto maxSpeed = 25.0f;
  if(keyMap[sf::Keyboard::W]) {
    camSpeed += maxSpeed;
  }
  if(keyMap[sf::Keyboard::S]) {
    camSpeed -= maxSpeed;
  }
  if(keyMap[sf::Keyboard::A]) {
    strafeSpeed -= maxSpeed;
  }
  if(keyMap[sf::Keyboard::D]) {
    strafeSpeed += maxSpeed;
  }

  if(keyMap[sf::Keyboard::Space]) {
    auto deltaMouse = sf::Vector2i(window.getSize()) / 2 - sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window.getSize()) / 2, window);
    auto sensitivity = 1.2f;
    rotateHor = deltaMouse.x * sensitivity * deltaTime;
    rotateVert = deltaMouse.y * sensitivity * deltaTime;
  }

  static glm::vec2 mouseLastPos;
  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
  }

  if(keyMap[sf::Keyboard::X]) {
    if(!shaderReloaded) {
      glDeleteProgram(shaderID);
      shaderID = LoadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
      getShaderUniforms(shaderID);

      shaderReloaded = true;
    }
  } else {
    shaderReloaded = false;
  }

  if(keyMap[sf::Keyboard::T]) {
    zoomOutAnimation = true;
  }

  if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {

    auto mousePos = sf::Vector2f(sf::Mouse::getPosition(window));
    mousePos.x /= float(window.getSize().x) / 2.0f;
    mousePos.y /= float(window.getSize().y) / 2.0f;
    auto pos = mousePos - sf::Vector2f(1.0f, 1.0f);
    pos.y /= getAspectRatio();
    if(!mouseDrag) {
      pickedPoint = (centerPoint + glm::vec2(pos.x, -pos.y)*zoom);
    }
    mouseDrag = true;
    centerPoint = (pickedPoint - glm::vec2(pos.x, -pos.y)*zoom);
    printf("%f %f\n", centerPoint.x, centerPoint.y);
  } else {
    mouseDrag = false;
  }

  if(zoomOutAnimation) {
    auto zoomMultiplier = 1.0f + 0.1f;
    if(zoom < 1.0f) {
     zoom *= zoomMultiplier; 
     if(zoom >= 1.0f) {
       zoomOutAnimation = false;
       zoom = 1.0f;
     }
    }
    if(zoom > 1.0f) {
     zoom /= zoomMultiplier; 
     if(zoom <= 1.0f) {
       zoom = 1.0f;
       zoomOutAnimation = false;
     }
    }
  }

  camSpeed *= deltaTime;
  strafeSpeed *= deltaTime;
  camera.horAngle() += glm::radians(rotateHor);
  camera.vertAngle() += glm::radians(rotateVert);
  camera.move(camera.dir() * camSpeed + camera.right() * strafeSpeed);
}

void App::draw() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  camera.update();
  viewMatrix = glm::lookAt(
      camera.pos(),
      camera.pos() + camera.dir(),
      camera.up()
    );
  projectionView = projectionMatrix * viewMatrix;

  glUseProgram(shaderID);
  glBindVertexArray(vao);

  glUniformMatrix4fv(viewID, 1, GL_FALSE, &viewMatrix[0][0]);
  glUniformMatrix4fv(projectionID, 1, GL_FALSE, &projectionMatrix[0][0]);
  glUniform1f(timeUniform, startClock.getElapsedTime().asSeconds());
  glUniform1f(zoomUniform, zoom);
  glUniform2f(centerUniform, centerPoint.x, centerPoint.y);
  glUniform2f(resolutionUniform, window.getSize().x, window.getSize().y);

  glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 3);

  glBindVertexArray(0);
  window.pushGLStates();

  window.popGLStates();
  window.display();
}

void App::handleResize() {
  glViewport(0, 0, window.getSize().x, window.getSize().y);
  projectionMatrix = glm::perspective(
      glm::radians(45.0f),
      getAspectRatio(),
      0.1f,
      300.0f
    );
}

void App::mainLoop() {
  sf::Time tickTime = sf::seconds(1.0f / static_cast<float>(tickCount));
  sf::Time timeAccumulator = sf::seconds(0.0f);
  sf::Time lastFrameTime   = sf::seconds(0.0f);
  sf::Clock timer;

  startClock.restart();
  timer.restart();
  fpsClock.restart();
  handleResize();

  while(window.isOpen()) {
    sf::Time timeDifference = timer.getElapsedTime() - lastFrameTime;
    timeAccumulator += timeDifference;
    lastFrameTime = timer.getElapsedTime();

    handleEvents();

    while(timeAccumulator >= tickTime) {
      logic(tickTime.asSeconds());
      timeAccumulator -= tickTime;
      #ifndef NDEBUG
        actualTickCount++;
      #endif
    }

    draw();

    #ifndef NDEBUG
      showDebugInfo();
    #endif
  }
}

void App::showDebugInfo() {
  fpsCount++;
  if(fpsClock.getElapsedTime() > sf::seconds(1.0f)) {
    window.setTitle(
        titlePrefix
      + "FPS: "
      + std::to_string(fpsCount)
      + " TICKS: "
      + std::to_string(actualTickCount)
    );
    fpsClock.restart();
    fpsCount = 0;
    actualTickCount = 0;
  }
}


void App::getShaderUniforms(GLuint shaderID) {
  viewID       = glGetUniformLocation(shaderID, "view");
  projectionID = glGetUniformLocation(shaderID, "projection");
  timeUniform  = glGetUniformLocation(shaderID, "time");
  zoomUniform  = glGetUniformLocation(shaderID, "zoom");
  centerUniform  = glGetUniformLocation(shaderID, "centerPoint");
  resolutionUniform  = glGetUniformLocation(shaderID, "resolution");
}