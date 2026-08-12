#pragma once
#include <vector>
#include "NPC.h"
#include "SpriteManager.h"

class AILogicManager {
private:
    std::vector<NPC*> fishPool;

public:
    AILogicManager(int poolSize = 50); //set this to account for the total amount of npc able to be spawned at same time
    ~AILogicManager();

    void SpawnFish(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points, SpriteData sprite);
    void Update();
    void Render(SpriteManager* spriteManager);

    std::vector<NPC*>& GetFishes() { return fishPool; }
};