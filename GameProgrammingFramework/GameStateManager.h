#pragma once
#include "IGameState.h"
#include "PlayerInput.h"
#include <vector>

class GameStateManager {
private:
    std::vector<IGameState*> stateStack;
    int pendingPops;
    IGameState* pendingPush;

public:
    GameStateManager();
    ~GameStateManager();

    void ChangeState(IGameState* newState);
    void PushState(IGameState* newState);
    void PopState(IGameState* newState);
    void PopState();

    void HandleInput();
    void Update(float deltaTime);
    void Render();
};