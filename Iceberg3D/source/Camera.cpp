#include "Camera.h"
#include "Game.h"

using namespace iceberg;
   
Camera::Camera(Game* game, glm::vec3 position, glm::vec3 focusPoint, float speed)
{
    // Initialize variables
    direction_ = glm::vec3(0, 0, 0);
    right_ = glm::vec3(0, 0, 0);
    up_ = glm::vec3(0, 0, 0);

    speed_ = speed;
    position_ = position;

    fieldOfView_ = 45.0f;
    pitchSensitivity_ = 0.005f;
    yawSensitivity_ = 0.005f;

    horizontalAngle_ = glm::pi<float>();
    verticalAngle_ =  0.0f;

    nearClippingPlane_ = 0.1f;
    farClippingPlane_ = 10000.0f;
 
    mouseX_ = 320; // TODO: screenWidth/2
    mouseY_ = 240; // TODO: screenheight/2
    
    inputEnabled_ = false;

    // TODO: Hide cursor
    // TODO: set cursor to center of screen

    calculate_vectors(game);
    LookAt(game, position_, focusPoint, up_);
}

void Camera::calculate_vectors(Game* game)
{
    // TODO: Get the cursor's x and y position
    // TODO: Set the cursor's x and y position to center of screen

    // TODO: Set these to the center of the screen in x and y (width/2, height/2)
    float centerX = 320;
    float centerY = 240;

    horizontalAngle_ += yawSensitivity_ * float(centerX - mouseX_);
    verticalAngle_ += pitchSensitivity_ * float(centerY - mouseY_);

    // 1.55f radians is 89 degrees, which is a reasonable vertical constraint
    if (verticalAngle_ > 1.55f)
        verticalAngle_ = 1.55f;
    else if (verticalAngle_ < -1.55f)
        verticalAngle_ = -1.55f;

    direction_ = glm::vec3(
        cos(verticalAngle_) * sin(horizontalAngle_),
        sin(verticalAngle_),
        cos(verticalAngle_) * cos(horizontalAngle_)
    );

    right_ = glm::vec3(
        sin(horizontalAngle_ - glm::pi<float>() / 2),
        0,
        cos(horizontalAngle_ - glm::pi<float>() / 2)
    );

    up_ = cross(right_, direction_);
}

void Camera::update(Game *game)
{
    if (!inputEnabled_) return;

    calculate_vectors(game);

// TODO: Set input properly using SDL
/*
    // Move forward
    if (inputManager->key_pressed(ICEBERG_KEY_W))
    {
        position_ += direction_ * speed_ * game->delta_time();
    }

    // Move backward
    if (inputManager->key_pressed(ICEBERG_KEY_S))
    {
        position_ -= direction_ * speed_ * game->delta_time();
    }

    // Strafe left
    if (inputManager->key_pressed(ICEBERG_KEY_A))
    {
        position_ -= right_ * speed_ * game->delta_time();
    }

    // Strafe right
    if (inputManager->key_pressed(ICEBERG_KEY_D))
    {
        position_ += right_ * speed_ * game->delta_time();
    }
*/

    LookAt(game, position_, position_ + direction_, up_);
}

glm::mat4 Camera::make_mvp(glm::mat4 modelMatrix) const
{
    glm::mat4 mvpMatrix = projectionMatrix_ * viewMatrix_ * modelMatrix;
    return mvpMatrix;
}

void Camera::enable_input(bool enabled)
{
    inputEnabled_ = enabled;
}

void Camera::LookAt(Game *game, glm::vec3 position, glm::vec3 focusPoint, glm::vec3 up)
{
    // TODO: Set the projection matrix properly, getting aspect ratio of current window
    //projectionMatrix_ = glm::perspective(fieldOfView_, aspectRatio, nearClippingPlane_, farClippingPlane_);
    position_ = position;
    viewMatrix_ = glm::lookAt(position_, focusPoint, up_);
}

glm::mat4 Camera::projection_matrix() const
{
    return projectionMatrix_;
}

glm::mat4 Camera::view_matrix() const
{
    return viewMatrix_;
}

