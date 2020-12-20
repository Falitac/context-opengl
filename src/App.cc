#include "../include/App.hpp"

App::App():
contextSettings(24, 8, 4, 4, 6),
titlePrefix("OpenGL "),
camera(glm::vec3(0.f, 0.f, -7.f)),
cubeScaleFactor(1.0f),
animation(false)
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
  glClearColor(0.3f, 0.5f, 0.3f, 1.0f);

  vertices = genCube();
  indices = {
    7, 3, 1,
    7, 1, 5,

    3, 2, 0,
    3, 0, 1,

    2, 6, 4,
    2, 4, 0,

    6, 7, 5,
    6, 5, 4, 

    7, 6, 2,
    0, 5, 1,

    3, 7, 2,
    0, 4, 5,
  };

  std::uniform_real_distribution<float> dist;

  for(int i=0; auto& vertex: vertices) {
    colors.emplace_back(dist(rand));

    if(i%3 == 2) {
      std::cout << "\n";
    } else {
      std::cout << " ";
    }
    i++;
  }

  glGenBuffers(1, &verticesBuffer);
  glGenBuffers(1, &colorBuffer);
  glGenBuffers(1, &indicesBuffer);

  glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    vertices.size()*sizeof(float),
    vertices.data(),
    GL_STATIC_DRAW
  );
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glBufferData(
    GL_ARRAY_BUFFER,
    colors.size()*sizeof(float),
    colors.data(),
    GL_STATIC_DRAW
  );
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
  glBufferData(
    GL_ELEMENT_ARRAY_BUFFER,
    indices.size()*sizeof(GLuint),
    indices.data(),
    GL_STATIC_DRAW
  );

  shaderID = LoadShaders("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");

  mvpID = glGetUniformLocation(shaderID, "MVP");
  //viewID = glGetUniformLocation(shaderID, "view");
  //modelID = glGetUniformLocation(shaderID, "model");


  mainLoop();
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
  if(keyMap[sf::Keyboard::X]) {
    animation = true;
    animationClock.restart();
  }

  if(keyMap[sf::Keyboard::Space]) {
    auto deltaMouse = sf::Vector2i(window.getSize()) / 2 - sf::Mouse::getPosition(window);
    sf::Mouse::setPosition(sf::Vector2i(window.getSize()) / 2, window);
    auto sensitivity = 1.2f;
    rotateHor = deltaMouse.x * sensitivity * deltaTime;
    rotateVert = deltaMouse.y * sensitivity * deltaTime;
  }

  if(animation) {
    auto t1 = sf::seconds(0.5f);
    auto t2 = sf::seconds(1.2f);
    auto animationFuncAsc = [&](const float& time) {
      auto mapped = time / t1.asSeconds();
      return 2.0f - glm::pow((mapped - 1.f), 2);
    };
    auto animationFuncDesc = [&](const float& time) {
      auto mapped =  (time - t1.asSeconds()) / (t2 - t1).asSeconds();
      return 1.0f + glm::pow((mapped - 1.f), 2);
    };
    auto animationTime = animationClock.getElapsedTime();
    if(animationTime <= t1) {
      cubeScaleFactor = animationFuncAsc(animationTime.asSeconds());
    } else if (animationTime <= t2) {
      cubeScaleFactor = animationFuncDesc(animationTime.asSeconds());
    } else {
      animation = false;
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

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, verticesBuffer);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);


  auto radius = 12;
  for(int y = -radius; y <= radius; y++) {
    float dAlfa = glm::abs(radius - y) + 1 / (glm::two_pi<float>() * radius);
    dAlfa = 0.1;
    for(float alfa = 0; alfa < glm::two_pi<float>(); alfa += dAlfa) {
      auto innerScale = 3;
      auto cubePos = glm::vec3();
      cubePos.y = y * innerScale;

      auto distance = glm::sqrt(radius*radius - y*y);
      distance *= innerScale;
      cubePos.x = distance * glm::sin(alfa);
      cubePos.z = distance * glm::cos(alfa);
      glm::mat4 model = glm::mat4(1.0f);
      model = glm::translate(model, cubePos);
      model = glm::scale(model, glm::vec3(cubeScaleFactor));
      model = glm::rotate(
                        model,
                        startClock.getElapsedTime().asSeconds(),
                        glm::vec3(0.2f, 1.0f, 0.5f)
                      );
      model = projectionView * model;
      glUniformMatrix4fv(mvpID, 1, GL_FALSE, &model[0][0]);
      glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }
  }


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
    sf::Time timeDiffrence = timer.getElapsedTime() - lastFrameTime;
    timeAccumulator += timeDiffrence;
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

