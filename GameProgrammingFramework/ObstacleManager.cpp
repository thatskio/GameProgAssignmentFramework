#include "ObstacleManager.h"



//ObstacleManager::ObstacleManager(int poolSize)
//	: maxObstacles(poolSize)
//{
//	obstaclePool.reserve(poolSize);
//}

ObstacleManager::ObstacleManager()
{

}

ObstacleManager::~ObstacleManager()
{
	for (Obstacle* obstacle : obstaclePool)
	{
		delete obstacle;
	}
	obstaclePool.clear();
}



void ObstacleManager::Update()
{
	for (Obstacle* obstacle : obstaclePool)
	{
		if (obstacle)
		{
			obstacle->Update();
		}
	}
}

void ObstacleManager::Render(LineManager* lineManager)
{
	for (Obstacle* obstacle : obstaclePool)
	{
		if (obstacle)
		{
			obstacle->Draw(lineManager, D3DCOLOR_ARGB(255, 100, 100, 100), 2.0f);
		}
	}
}

void ObstacleManager::AddObstacle()
{
	AddObstacle({
		D3DXVECTOR2(250.0f, 200.0f),
		D3DXVECTOR2(450.0f, 200.0f),
		D3DXVECTOR2(450.0f, 300.0f),
		D3DXVECTOR2(500.0f, 500.0f),
		D3DXVECTOR2(250.0f, 500.0f)
	});
}



void ObstacleManager::AddObstacle(const std::vector<D3DXVECTOR2>& nodePositions)
{
		obstaclePool.push_back(new Obstacle(nodePositions));

}