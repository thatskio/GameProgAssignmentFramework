#pragma once
#include "NPC.h"
#include "SpriteManager.h"
#include <vector> //To use arrays

class AILogicManager {
private:
    std::vector<NPC*> fishPool; //Creating an array to hold all fish instances

public:
    AILogicManager(int poolSize = 50); //Set this to account for the total amount of npc/fish able to be spawned at same time
    ~AILogicManager();

    void SpawnFish(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points, SpriteData sprite);
    void Update();
    void Render(SpriteManager* spriteManager);

    std::vector<NPC*>& GetFishes() { return fishPool; }
};