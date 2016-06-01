#include "Game.h"

using namespace iceberg;

Game::Game(Backend backend)
    :stateMachine_(std::make_shared<StateMachine>())
{
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not be initialized!\n SDL_Error: %s\n", SDL_GetError());
        return;
        // TODO: throw
    }
    else
    {
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        SDL_Window* window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);

        if(window == nullptr)
        {
            printf("Window could not be created!\n SDL_Error: %s\n", SDL_GetError());
            return;
            // TODO: throw
        }
        else
        {
            SDL_GLContext context = SDL_GL_CreateContext(window);
            if(context == nullptr)
            {
                printf("OpenGL context could not be created!\n SDL_Error: %s\n", SDL_GetError());
                return;
            }
            else
            {
                glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
            }
        }
    }

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
