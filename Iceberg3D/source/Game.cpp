#include "Game.h"

using namespace iceberg;

Game::Game(Backend backend)
    :stateMachine_(std::make_shared<StateMachine>())
{
    // TODO: Initialize SDL here
    switch(backend)
    {
    case OPENGL:
        break;

    default:
        throw std::runtime_error("Invalid backend specified");
    }

    // Begin the timer for delta time calculation
    previousTime_ = std::chrono::high_resolution_clock::now();

    running_ = false;
}

Game::~Game()
{
}

void Game::change_state(GameState* state) const
{
    stateMachine_->change_state(state);
}

void Game::update()
{
    if (!running_) return;

    currentTime_ = std::chrono::high_resolution_clock::now();
    deltaTime_ = std::chrono::duration_cast<std::chrono::duration<float>>(currentTime_ - previousTime_).count();
    previousTime_ = std::chrono::high_resolution_clock::now();

    // TODO: Update Windows
    // TODO: Check if last or main window is closed 
    // TODO: Set running_ to false if there are no windows left, or if flagged for exit 

    stateMachine_->update();
}

void Game::draw() const
{
    if (!running_) return;

    stateMachine_->draw();

    //TODO: Swap buffers
}

void Game::run()
{
    // TODO: Modify game loop here
    running_ = true;
    while (running_)
    {
        update();
        draw();
    }
}

float Game::delta_time() const
{
    return deltaTime_;
}

void Game::handle_error(const std::string &errorMessage)
{
    // TODO: Handle errors, preferably with SDL MessageBoxes
}
