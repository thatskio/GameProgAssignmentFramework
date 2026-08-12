#pragma once

class GameStateManager;

class IGameState {
protected:
    GameStateManager* handler;

public:
    IGameState(GameStateManager* handlerPtr) : handler(handlerPtr) {}
    virtual ~IGameState() {} // Ensures proper cleanup of child states

    virtual void Input() = 0;
    virtual void Update(float deltaTime) = 0; // Passing deltaTime for smooth physics
    virtual void Render() = 0;
};