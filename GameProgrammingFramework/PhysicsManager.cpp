#include "PhysicsManager.h"
#include <algorithm> // this is for std::min and std::max
#include <limits>
#include <cstdlib>
#include <cmath>

PhysicsManager::PhysicsManager() {}
PhysicsManager::~PhysicsManager() {}

//If the distance squared between 2 objects is less than the combined radius squared, means collision = true
bool PhysicsManager::SimpleCircleCollision(GameObject* objA, GameObject* objB) {
    D3DXVECTOR2 objACenter = objA->GetPosition() + D3DXVECTOR2(
        (objA->GetSprite().rect.right - objA->GetSprite().rect.left) * objA->GetScale().x * 0.5f,
        (objA->GetSprite().rect.bottom - objA->GetSprite().rect.top) * objA->GetScale().y * 0.5f);
    D3DXVECTOR2 objBCenter = objB->GetPosition() + D3DXVECTOR2(
        (objB->GetSprite().rect.right - objB->GetSprite().rect.left) * objB->GetScale().x * 0.5f,
        (objB->GetSprite().rect.bottom - objB->GetSprite().rect.top) * objB->GetScale().y * 0.5f);

    float deltaX = objBCenter.x - objACenter.x;
    float deltaY = objBCenter.y - objACenter.y;
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

// Circle vs Polygon
void PhysicsManager::SATCollision(Projectile* projectile, Obstacle* obstacle) {
    //////////////////////////////////////////////////////////////////////////////
    // resources: 
    // https://dyn4j.org/2010/01/sat/
    // https://www.youtube.com/watch?v=XuxrUE-cHn4
    //////////////////////////////////////////////////////////////////////////////


    if (obstacle->GetNodeCount() < 2) {
        return;
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

        // the perp method is just (x, y) => (-y, x) or (y, -x)
        axes[i] = normal;

        D3DXVec2Normalize(&axes[i], &axes[i]);
    }

    D3DXVECTOR2 circlePos = projectile->GetCenter();

    // Add axis from closest vertex to circle center for corner collision
    int closestNode = 0;
    float minDistSq = (std::numeric_limits<float>::max)();
    for (int i = 0; i < obstacle->GetNodeCount(); i++) {
        D3DXVECTOR2 diff = circlePos - obstacle->GetNode(i);
        float distSq = D3DXVec2Dot(&diff, &diff);
        if (distSq < minDistSq) {
            minDistSq = distSq;
            closestNode = i;
        }
    }
    D3DXVECTOR2 cornerAxis = circlePos - obstacle->GetNode(closestNode);
    D3DXVec2Normalize(&cornerAxis, &cornerAxis);
    axes.push_back(cornerAxis);

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
			return; // no collision found
		}

        if (overlapAmount < minimumOverlap){
            minimumOverlap = overlapAmount;
			mtv = axis;
        }
        // NOTE TO SELF(or whoevers reading ts): The D3DXVECTOR here is not a vector for direction but for lines on a 1D plane.
         //circle.x = min, circle.y = max | polygon.x = min, polygon.y = max
        if (circle.y < polygon.x || polygon.y < circle.x) {
			return;     // no collision found
        }


    }

    // Ensure MTV points from Obstacle to Projectile
    D3DXVECTOR2 dir = circlePos - obstacle->GetCenter();
    if (D3DXVec2Dot(&dir, &mtv) < 0.0f) {
        mtv = -mtv;
    }

	std::cout << "Collision detected!" << std::endl;
    if (projectile->RegisterObstacleHit()) {
        projectile->SetActive(false);
    }
    if (projectile->GetMass() == 0.0f) {
        return;
    }
    projectile->SetPosition(projectile->GetPosition() + mtv * (float)minimumOverlap);

    D3DXVECTOR2 newVelocity = projectile->GetVelocity();
    float velocityNormal = D3DXVec2Dot(&newVelocity, &mtv);

    if (velocityNormal < 0.0f) {
        D3DXVECTOR2 newVel = newVelocity - 2 * velocityNormal * mtv / projectile->GetMass();
        projectile->SetVelocity(newVel);
    }


}
// Polygon vs Polygon
void PhysicsManager::SATCollision(Obstacle* obstacle1, Obstacle* obstacle2) {
    if (obstacle1->GetNodeCount() < 2 || obstacle2->GetNodeCount() < 2) {
        return;
    }

    std::vector<D3DXVECTOR2> axes;

    // Get Normal
    // Loop through polygon to find normal for each side (obstacle)
    Obstacle* obstacles[2] = { obstacle1, obstacle2 };
    for (int obj = 0; obj < 2; obj++) {
        Obstacle* obstacle = obstacles[obj];
        for (int i = 0; i < obstacle->GetNodeCount(); i++) {
            // get the current vertex
            D3DXVECTOR2 p1 = obstacle->GetNode(i);

            // get the next vertex
            D3DXVECTOR2 p2 = obstacle->GetNode(i + 1 == obstacle->GetNodeCount() ? 0 : i + 1);

            // subtract the two to get the edge vector
            D3DXVECTOR2 edge = p1 - p2;

            // get either perpendicular vector
            D3DXVECTOR2 normal = D3DXVECTOR2(-edge.y, edge.x);

            // the perp method is just (x, y) => (-y, x) or (y, -x)
            D3DXVec2Normalize(&normal, &normal);
            axes.push_back(normal);
        }
    }

    //////////////////////////////////////////////////////////////////////////////
    // Minimum Translation Vector
    // The minimum amount of vector needed to push the object out of collision.
    // If mtv = (0,0), no collision will be found
    D3DXVECTOR2 mtv(0, 0);
    double minimumOverlap = (std::numeric_limits<double>::max)();
    //////////////////////////////////////////////////////////////////////////////

    // Overlap Checking
    // Loop through axis to check for overlap
    for (size_t i = 0; i < axes.size(); i++) {
        D3DXVECTOR2 axis = axes[i];

        D3DXVECTOR2 polygon1 = PhysicsManager::Project(axis, *obstacle1);
        D3DXVECTOR2 polygon2 = PhysicsManager::Project(axis, *obstacle2);

        // NOTE TO SELF(or whoevers reading ts): The D3DXVECTOR here is not a vector for direction but for lines on a 1D plane.
        //circle.x = min, circle.y = max | polygon.x = min, polygon.y = max
        if (polygon1.y < polygon2.x || polygon2.y < polygon1.x) {
            return;     // no collision found
        }

        double overlapAmount = (std::min)(polygon1.y, polygon2.y) - (std::max)(polygon1.x, polygon2.x); // bracket on min max cuz win.h preprocessor doing some goofy shit

        if (overlapAmount <= 0) {
            return; // no collision found
        }

        if (overlapAmount < minimumOverlap) {
            minimumOverlap = overlapAmount;
            mtv = axis;
        }
    }

    // Ensure MTV points from Obstacle to Projectile
    D3DXVECTOR2 direction = obstacle1->GetCenter() - obstacle2->GetCenter();
    if (D3DXVec2Dot(&direction, &mtv) < 0.0f) {
        mtv = -mtv;
    }

    std::cout << "Collision detected!" << std::endl;
    if (obstacle1->GetMass() == 0.0f) {
        return;
    }
    obstacle1->SetPosition(obstacle1->GetPosition() + mtv * (float)minimumOverlap);

    D3DXVECTOR2 newVelocity = obstacle1->GetVelocity();
    float velocityNormal = D3DXVec2Dot(&newVelocity, &mtv);

    if (velocityNormal < 0.0f) {
        //float restitution = projectile->GetRestitution(); // 1.0 = fully elastic, 0.0 = stops
        D3DXVECTOR2 newVel = newVelocity - 2 * velocityNormal * mtv * obstacle1->GetMass();
        obstacle1->SetVelocity(newVel);
    }
}













void PhysicsManager::ProcessPhysics(AILogicManager* aiManager
    , BulletsManager* projManager, Player* player, AudioManager* audioManager, ObstacleManager* obstacleManager, int screenWidth, int screenHeight) {
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
            if (!bullet->IsActive()) { break; }
			SATCollision(bullet, obstacle);


        }
        if (!bullet->IsActive()) { continue; }



        //Bullet against fish collision
        for (NPC* fish : fishes) {
            if (!fish->IsActive()) { continue; }

            if (SimpleCircleCollision(bullet, fish)) {
                float deathChance = (100.0f - (std::abs(fish->GetScale().x) * 25.0f))/3;
                deathChance = (std::max)(0.0f, (std::min)(100.0f, deathChance));
                float randomChance = static_cast<float>(std::rand() % 10000) / 100.0f;

                if (randomChance < deathChance) {
                    fish->TakeDamage(bullet->GetDamage());
                }
                bullet->SetActive(false);

                if (audioManager) {
                    SoundParams soundParams;
                    float collisionX = bullet->GetCenter().x;
                    soundParams.pan = (std::max)(-1.0f, (std::min)(1.0f, (collisionX / screenWidth) * 2.0f - 1.0f));
                    audioManager->PlayAudio("Hit", soundParams);
                }

                if (!fish->IsActive()) {
                    int scaledPoints = static_cast<int>(fish->GetPointValue() * std::abs(fish->GetScale().x)*2);
                    player->AddScore(scaledPoints);
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
