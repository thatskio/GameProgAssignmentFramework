#include "Obstacle.h"

Obstacle::Obstacle()
{
}



Obstacle::~Obstacle() = default;

void Obstacle::Update()
{
}

void Obstacle::Draw(LineManager* lineManager, D3DCOLOR color, float thickness) const
{
	if (!lineManager || nodes.size() < 2) {
		return;
	}

	for (size_t i = 0; i < nodes.size(); ++i) {
		const D3DXVECTOR2& start = nodes[i];
		const D3DXVECTOR2& end = nodes[(i + 1) % nodes.size()];
		lineManager->DrawLine(start, end, thickness, color);
	}
}


D3DXVECTOR2 Obstacle::GetNode(size_t index) const {
	if (index < nodes.size()) {
		return nodes[index];
	}
	return D3DXVECTOR2(0.0f, 0.0f); // Return a default value if index is out of bounds
}

int Obstacle::GetNodeCount() const {
	return static_cast<int>(nodes.size());
}


bool Obstacle::HasNodes() const
{
	return !nodes.empty();
}

//returns the average center of all nodes
D3DXVECTOR2 Obstacle::GetCenter() const {
	int count = GetNodeCount();
	if (count == 0) return D3DXVECTOR2(0.0f, 0.0f);

	D3DXVECTOR2 center(0.0f, 0.0f);
	for (int i = 0; i < count; i++) {
		center += GetNode(i);
	}
	return center / (float)count;
}
