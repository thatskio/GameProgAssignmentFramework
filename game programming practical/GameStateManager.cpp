#include "GameStateManager.h"

GameStateManager::GameStateManager() {
    currentState = nullptr;
    nextState = nullptr;
}

GameStateManager::~GameStateManager() {
    if (currentState) delete currentState;
}

void GameStateManager::ChangeState(IGameState* newState) {
    nextState = newState;
}

void GameStateManager::Update(float deltaTime) {
    if (nextState != nullptr) {
        if (currentState != nullptr) {
            delete currentState;
        }
        currentState = nextState;
        nextState = nullptr;
    }
    if (currentState) currentState->Update(deltaTime);
}

void GameStateManager::HandleInput() {
    if (currentState) currentState->Input();
}

void GameStateManager::Render() {
    if (currentState) currentState->Render();
}