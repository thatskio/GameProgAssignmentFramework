#pragma once
#include "GameObject.h"
#include "LineManager.h"
#include <vector>

class Obstacle : public GameObject {
private:
    std::vector<D3DXVECTOR2> nodes;


public:
    Obstacle();
    Obstacle(const std::vector<D3DXVECTOR2>& nodePositions) : nodes(nodePositions) {}

    ~Obstacle();

    void Update() override;
    void Draw(LineManager* lineManager, D3DCOLOR color, float thickness) const;
    bool HasNodes() const;

    D3DXVECTOR2 GetCenter() const;

    D3DXVECTOR2 GetNode(size_t index) const;
    int GetNodeCount() const;

};
