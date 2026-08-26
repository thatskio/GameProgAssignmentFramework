#include "AILogicManager.h"

AILogicManager::AILogicManager(int poolSize) {
    for (int i = 0; i < poolSize; i++) {
        fishPool.push_back(new NPC());
    }
}

AILogicManager::~AILogicManager() {
    for (NPC* fish : fishPool) {
        delete fish;
    }
    fishPool.clear();
}

void AILogicManager::SpawnFish(D3DXVECTOR2 startPos, D3DXVECTOR2 startVel, int hp, int points, SpriteData sprite) {
    for (NPC* fish : fishPool) {
        if (!fish->IsActive()) {
            fish->SetSprite(sprite);
            fish->Spawn(startPos, startVel, hp, points);
            return;
        }
    }
}

void AILogicManager::Update() {
    for (NPC* fish : fishPool) {
        if (fish->IsActive()) fish->Update();
    }
}

void AILogicManager::Render(SpriteManager* spriteManager) {
    for (NPC* fish : fishPool) {
        if (fish->IsActive()) fish->Render(spriteManager);
    }
}