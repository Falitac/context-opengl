#pragma once

#include <glm/glm.hpp>
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

class Camera
{
    float fieldOfView;
    float verticalAngle;
    float horizontalAngle;
    glm::vec3 direction;
    glm::vec3 rightDirection;
    glm::vec3 upDirection;

    glm::vec3 position;
    
public:
    Camera(const glm::vec3& initPosition = glm::vec3(0.f, 0.f, 0.f));


    inline float& FOV() { return fieldOfView; };
    inline float& vertAngle() { return verticalAngle; };
    inline float& horAngle() { return horizontalAngle; };
    inline glm::vec3& pos() { return position; };

    const inline glm::vec3& dir() const { return direction; };
    const inline glm::vec3& right() const { return rightDirection; };
    const inline glm::vec3& up() const { return upDirection; };

    inline void setPosition(const glm::vec3& where)
    {
        position = where;
    }

    inline void move(const glm::vec3& deltaVector)
    {
        position += deltaVector;
    }

    inline glm::mat4 view()
    {
        return glm::lookAt(position, position + direction, upDirection);
    }

    void update();
};