#include "GameStateManager.h"

GameStateManager::GameStateManager() {
    pendingPops = 0;
    pendingPush = nullptr;
}

GameStateManager::~GameStateManager() {
    for (int i = (int)stateStack.size() - 1; i >= 0; i--) {     //Loop through the stack list to delete all game states
        if (stateStack[i]) {
            delete stateStack[i];
        }
    }
    stateStack.clear();
    if (pendingPush) { delete pendingPush; }                    //Clear out any game states requested to load, to prevent memory leak
}

void GameStateManager::ChangeState(IGameState* newState) {
    if ((int)stateStack.size() > pendingPops) {                 //Give a timer/cooldown to the Update() to update game state before continuing to render
        pendingPops++;
    }
    if (pendingPush != nullptr) { delete pendingPush; }
    pendingPush = newState;
}

void GameStateManager::PushState(IGameState* newState) {
    if (pendingPush != nullptr) { delete pendingPush; }         
    pendingPush = newState;
}

void GameStateManager::PopState() {
    if ((int)stateStack.size() > pendingPops) {                 //Give a timer/cooldown to the Update() to update game state before continuing to render
        pendingPops++;
    }
}

void GameStateManager::PopState(IGameState* newState) {         //Alternative PopState with NewState value to immediately run new game state after popping old one
    if ((int)stateStack.size() > pendingPops) {            
        pendingPops++;
    }
    GameStateManager::ChangeState(newState);
}


void GameStateManager::HandleInput() {
    if (!stateStack.empty()) {
        stateStack.back()->Input();
    }
}

void GameStateManager::Update(float deltaTime) {
    //Popping old states
    while (pendingPops > 0 && !stateStack.empty()) { //If in cooldown/timer
        delete stateStack.back();
        stateStack.pop_back();
        pendingPops--;
    }
    pendingPops = 0;

    //Pushing new states
    if (pendingPush != nullptr) {
        stateStack.push_back(pendingPush);
        pendingPush = nullptr;
    }

    //Updating/Rendering current state
    if (!stateStack.empty()) {
        stateStack.back()->Update(deltaTime);
    }
}

void GameStateManager::Render() {
    if (!stateStack.empty()) {
        stateStack.back()->Render();
    }
}