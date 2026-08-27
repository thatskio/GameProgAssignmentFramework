#include "PhysicsManager.h"
#include <algorithm> // this is for std::min and std::max
#include <limits>

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager() {}

//If the distance squared between 2 objects is less than the combined radius squared, means collision = true
bool PhysicsManager::SimpleCircleCollision(GameObject* objA, GameObject* objB) {
    float deltaX = objB->GetPosition().x - objA->GetPosition().x;
    float deltaY = objB->GetPosition().y - objA->GetPosition().y;
    float squaredDistance = (deltaX * deltaX) + (deltaY * deltaY);
    float combinedRadius = objA->GetRadius() + objB->GetRadius();

    return squaredDistance < (combinedRadius * combinedRadius);
}

D3DXVECTOR2 PhysicsManager::Project(const D3DXVECTOR2& axis, const Obstacle& obstacle)
{
    D3DXVECTOR2 node = obstacle.GetNode(0);
    double min = D3DXVec2Dot(&node, &axis);
    double max = min;
    for (int i = 1; i < obstacle.GetNodeCount(); i++) {
        node = obstacle.GetNode(i);
        double p = D3DXVec2Dot(&node, &axis);
        if (p < min) {
            min = p;
        }
        else if (p > max) {
            max = p;
        }
    }
    D3DXVECTOR2 projection = D3DXVECTOR2(min, max);
    return projection;

}

D3DXVECTOR2 PhysicsManager::SATCollision(Projectile* projectile, Obstacle* obstacle) {
    //////////////////////////////////////////////////////////////////////////////
    // resources: 
    // https://dyn4j.org/2010/01/sat/
    // https://www.youtube.com/watch?v=XuxrUE-cHn4
    //////////////////////////////////////////////////////////////////////////////


    if (obstacle->GetNodeCount() < 2) {
        return D3DXVECTOR2(0, 0);
    }

    std::vector<D3DXVECTOR2> axes(obstacle->GetNodeCount());
    
    // Get Normal
    // Loop through polygon to find normal for each side (obstacle)
    for (int i = 0; i < obstacle->GetNodeCount(); i++) {
        // get the current vertex
        D3DXVECTOR2 p1 = obstacle->GetNode(i);

        // get the next vertex
        D3DXVECTOR2 p2 = obstacle->GetNode(i + 1 == obstacle->GetNodeCount() ? 0 : i + 1);

        // subtract the two to get the edge vector
        D3DXVECTOR2 edge = p1 - p2;

        // get either perpendicular vector
        D3DXVECTOR2 normal = D3DXVECTOR2(-edge.y, edge.x);

        // the perp method is just (x, y) =&gt; (-y, x) or (y, -x)
        axes[i] = normal;

        D3DXVec2Normalize(&axes[i], &axes[i]);
    }

    D3DXVECTOR2 circlePos = projectile->GetCenter();
    //D3DXVECTOR2 closestVertex = obstacle->GetNode(0);
    //D3DXVECTOR2 toClosestVertex = circlePos - closestVertex;
    //float closestDistance = D3DXVec2LengthSq(&toClosestVertex);
    //for (int i = 1; i < obstacle->GetNodeCount(); i++) {
    //    D3DXVECTOR2 vertex = obstacle->GetNode(i);
    //    D3DXVECTOR2 toVertex = circlePos - vertex;
    //    float distance = D3DXVec2LengthSq(&toVertex);
    //    if (distance < closestDistance) {
    //        closestDistance = distance;
    //        closestVertex = vertex;
    //    }
    //}

    //D3DXVECTOR2 circleToVertex = closestVertex - circlePos;
    //if (D3DXVec2LengthSq(&circleToVertex) > 0.0f) {
    //    D3DXVec2Normalize(&circleToVertex, &circleToVertex);
    //    axes.push_back(circleToVertex);
    //}
    
    //////////////////////////////////////////////////////////////////////////////
    // Minimum Translation Vector
    // The minimum amount of vector needed to push the object out of collision.
    // If mtv = (0,0), no collision will be found
	D3DXVECTOR2 mtv(0, 0);
    double minimumOverlap = (std::numeric_limits<double>::max)();
    //////////////////////////////////////////////////////////////////////////////


    // Overlap Checking
    // Loop through axis to check for overlap
    for (int i = 0; i < axes.size(); i++) {
        D3DXVECTOR2 axis = axes[i];

        // Cirle projection onto axis
        float centerProjection = D3DXVec2Dot(&circlePos, &axis);
        float radius = projectile->GetRadius();
        D3DXVECTOR2 circle = D3DXVECTOR2(centerProjection - radius, centerProjection + radius);


        D3DXVECTOR2 polygon = PhysicsManager::Project(axis, *obstacle);
		double overlapAmount = (std::min)(circle.y, polygon.y) - (std::max)(circle.x, polygon.x); // bracket on min max cuz win.h preprocessor doing some goofy shit

		if (overlapAmount <= 0) {
			return D3DXVECTOR2(0, 0); // no collision found
		}

        if (overlapAmount < minimumOverlap){
            minimumOverlap = overlapAmount;
			mtv = axis;
        }
        // NOTE TO SELF(or whoevers reading ts): The D3DXVECTOR here is not a vector for direction but for lines on a 1D plane.
        // circle.x = min, circle.y = max | polygon.x = min, polygon.y = max
   //     if (circle.y < polygon.x || polygon.y < circle.x) {
			//return mtv;     // no collision found = mtv = (0,0)
   //     }


    }

	std::cout << "Collision detected!" << std::endl;
    return mtv * minimumOverlap; // self explanatory, if collision is found it will return the mtv to add to the projectile velocity for collision response.

}


void PhysicsManager::ProcessPhysics(AILogicManager* aiManager, BulletsManager* projManager, Player* player, AudioManager* audioManager, ObstacleManager* obstacleManager, int screenWidth, int screenHeight) {
    std::vector<Projectile*>& bullets = projManager->GetBullets();
    std::vector<NPC*>& fishes = aiManager->GetFishes();
    std::vector<Obstacle*>& obstacles = obstacleManager->GetObstacles();

    
    // Compute collisions for bullets
    for (Projectile* bullet : bullets) {
        if (!bullet->IsActive()) { continue; }

        D3DXVECTOR2 bPos = bullet->GetCenter();

        //////////////////////////////////////////////////////////////////////////////////////
        // DEPRECATED DUE TO BULLETS NOT LEAVING SCREEN BOUNDARIES
        //if (bPos.x < 0 || bPos.x > screenWidth || bPos.y < 0 || bPos.y > screenHeight) {
        //    bullet->SetActive(false);
        //    continue;
        //}
        //////////////////////////////////////////////////////////////////////////////////////

		//Bullet against obstacle collision
        for (Obstacle* obstacle : obstacles) {
			D3DXVECTOR2 objectPosition = bullet->GetPosition();
			D3DXVECTOR2 objectVelocity = bullet->GetVelocity();
			D3DXVECTOR2 mtv = SATCollision(bullet, obstacle);

			//bullet->SetPosition(objectPosition + mtv);
			if (mtv != D3DXVECTOR2(0, 0)) {
			std::cout << "MTV: (" << mtv.x << ", " << mtv.y << ")" << std::endl;
			bullet->SetPosition(objectPosition + mtv);
			bullet->SetVelocity(objectVelocity + mtv); // Reverse the velocity of the bullet
			}

			//if (SATCollision(bullet, obstacle)) {
			//	bullet->SetActive(false);
			//	std::cout << "Bullet collided with obstacle!" << std::endl;
			//}
        }



        //Bullet against fish collision
        for (NPC* fish : fishes) {
            if (!fish->IsActive()) { continue; }

            if (SimpleCircleCollision(bullet, fish)) {
                fish->TakeDamage(bullet->GetDamage());
                bullet->SetActive(false);

                if (audioManager) {
                    audioManager->PlayAudio("Boing");
                }

                if (!fish->IsActive()) {
                    player->AddScore(fish->GetPointValue());
                }
                break;
            }
        }
    }

    for (NPC* fish : fishes) {
        if (!fish->IsActive()) { continue; }

        D3DXVECTOR2 fPos = fish->GetPosition();
        D3DXVECTOR2 fVel = fish->GetVelocity();
        float radius = fish->GetRadius();

        if (fPos.x - radius < 0) {
            fPos.x = radius;
            fVel.x *= -1.0f;
        }
        else if (fPos.x + radius > screenWidth) {
            fPos.x = screenWidth - radius;
            fVel.x *= -1.0f;
        }

        if (fPos.y - radius < 0) {
            fPos.y = radius;
            fVel.y *= -1.0f;
        }
        else if (fPos.y + radius > screenHeight) {
            fPos.y = screenHeight - radius;
            fVel.y *= -1.0f;
        }

        fish->SetPosition(fPos);
        fish->SetVelocity(fVel);
    }

}
