#pragma once
#include "Obstacle.h"
#include "LineManager.h"
#include <vector>

class ObstacleManager
{
private:
    std::vector<Obstacle*> obstaclePool;
    //int maxObstacles;

public:
    //ObstacleManager(int poolSize = 50);      //Create the array size (and give it a max value)
    ObstacleManager();      //Create the array size (and give it a max value)
    
    ~ObstacleManager();

    void AddObstacle();
    void AddObstacle(const std::vector<D3DXVECTOR2>& nodePositions);
    
    void Update();
    void Render(LineManager* lineManager);

    std::vector<Obstacle*>& GetObstacles() { return obstaclePool; }   //Return the obstacle instances in an array
};

