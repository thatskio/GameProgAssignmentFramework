#pragma once

class GameStateManager;

class IGameState {
protected:
    GameStateManager* handler;

public:
    IGameState(GameStateManager* handlerPtr) : handler(handlerPtr) {}
    virtual ~IGameState() {} 

    virtual void Input() = 0;
    virtual void Update(float deltaTime) = 0; //Passing deltaTime for smooth physics rendering 
    virtual void Render() = 0;
};