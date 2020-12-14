Enemy AI implementation using Unreal Behavior Tree feature.

EnemyCharacter.h, EnemyCharacter.cpp
Character class for enemy. Contains behavior tree, blackboard and other components.

EnemyAIController.h, EnemyAIController.cpp
Definition of some methods about actions that enemy can take.
When player seen, chase player. When player collides, send player to spawn point.
If cannot see the player, go find closest patrol point.

BTSelectPatrolPoint.h, BTSelectPatrolPoint.cpp
Behavior Tree node which choose next patrol point.

CanSeeTarget.h, CanSeeTarget.cpp
Unreal behavior tree service which check whether can see player or not.
If cannot, call FindClosestPoint function.