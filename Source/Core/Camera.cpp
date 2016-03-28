#include "Camera.h"
#include "Game.h"

using namespace std;
using namespace glm;

// Grab the game instance
Game *game = Game::GetInstance();

Camera::Camera(vec3 position_, glm::vec3 focusPoint, float speed_)
{
    // Initialize variables
    direction_ = vec3(0, 0, 0);
    right_ = vec3(0, 0, 0);
    up_ = vec3(0, 0, 0);

    horizontalAngle_ = float(pi<float>());
    verticalAngle_ = 0.0f;
    initialFOV_ = 45.0f;
    this->speed_ = speed_;
    pitchSensitivity_ = 0.005f;
    yawSensitivity_ = 0.005f;

    mouseX_ = game->GetScreenWidth() / 2;
    mouseY_ = game->GetScreenHeight() / 2;

    inputEnabled_ = false;

    calculate_vectors(mouseX_, mouseY_);
    LookAt(position_, focusPoint, up_);
}

// Calculate direction_, right_, and up_ vectors based on mouse offsets interpreted as angles
void Camera::calculate_vectors(int mouseX_, int mouseY_)
{
    // up_date angles
    horizontalAngle_ += yawSensitivity_   * float((game->GetScreenWidth() / 2) - mouseX_);
    verticalAngle_   += pitchSensitivity_ * float((game->GetScreenHeight() / 2) - mouseY_);

    // 1.55f radians is 89 degrees, which is a reasonable vertical constraint
    if (verticalAngle_ > 1.55f)
        verticalAngle_ = 1.55f;
    else if (verticalAngle_ < -1.55f)
        verticalAngle_ = -1.55f;

    // Set new direction_ by converting spherical coordinates to cartesian
    direction_ = vec3(
        cos(verticalAngle_) * sin(horizontalAngle_),
        sin(verticalAngle_),
        cos(verticalAngle_) * cos(horizontalAngle_)
        );

    // Calculate right_ vector
    right_ = vec3(
        sin(horizontalAngle_ - pi<float>() / 2),
        0,
        cos(horizontalAngle_ - pi<float>() / 2)
        );

    // Calculate up_ vector
    up_ = cross(right_, direction_);
}

void Camera::update()
{
    if (!inputEnabled_)
    {
        return;
    }

    // Get keyboard state
    const Uint8 *keystates = SDL_GetKeyboardState(nullptr);

    // Hide the mouse
    SDL_ShowCursor(SDL_DISABLE);

    // If the mouse moved
    SDL_GetMouseState(&mouseX_, &mouseY_);

    // Reset mouse to center of the screen
    SDL_WarpMouseInWindow(game->GetWindow(), game->GetScreenWidth() / 2, game->GetScreenHeight() / 2);
    
    // Calculate up_, right_, and direction_ vectors
    calculate_vectors(mouseX_, mouseY_);

    // Move forward if 'W' is pressed
    if (keystates[SDL_SCANCODE_W])
    {
        position_ += direction_*speed_;
    }

    // Move backward if 'S' is pressed
    if (keystates[SDL_SCANCODE_S])
    {
        position_ -= direction_*speed_;
    }

    // Strafe left if 'D' is pressed
    if (keystates[SDL_SCANCODE_D])
    {
        position_ += right_*speed_;
    }

    // Strafe right_ if 'A' is pressed
    if (keystates[SDL_SCANCODE_A])
    {
        position_ -= right_*speed_;
    }

    projectionMatrix_ = perspective(initialFOV_, game->GetAspectRatio(), 0.1f, 1000.0f);
    viewMatrix_ = lookAt(position_, position_ + direction_, up_);
}

float Camera::pitch() const
{
    return pitchSensitivity_;
}

float Camera::yaw() const
{
    return yawSensitivity_;
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

void Camera::LookAt(glm::vec3 position_, glm::vec3 focusPoint, glm::vec3 up_)
{
    this->projectionMatrix_ = perspective(initialFOV_, Game::GetInstance()->GetAspectRatio(), 0.1f, 1000.0f);
    this->position_ = position_;
    this->viewMatrix_ = glm::lookAt(position_, focusPoint, up_);
}

glm::mat4 Camera::projection_matrix() const
{
    return projectionMatrix_;
}

glm::mat4 Camera::view_matrix() const
{
    return viewMatrix_;
}
