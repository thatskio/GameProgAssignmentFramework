#pragma once
#include "IGameState.h"

class GameStateManager {
private:
    IGameState* currentState;
    IGameState* nextState;

public:
    GameStateManager();
    ~GameStateManager();

    void ChangeState(IGameState* newState);
    void HandleInput();
    void Update(float deltaTime);
    void Render();
};