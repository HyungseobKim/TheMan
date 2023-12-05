Enemy AI implementation using Unreal Behavior Tree feature.

EnemyCharacter.h, EnemyCharacter.cpp
- Character class for enemy. Contains behavior tree, blackboard and other components.

EnemyAIController.h, EnemyAIController.cpp
- Definition of some methods about actions that the enemy can take.
- When the enemy sees the player, it chases the player.
- When the player collides with the enemy, the player will be sent to the restart point.
- If the enemy cannot see the player, go back to the closest patrol point.

BTSelectPatrolPoint.h, BTSelectPatrolPoint.cpp
- Behavior Tree node which choose next patrol point.

CanSeeTarget.h, CanSeeTarget.cpp
- Unreal behavior tree service which checks whether the enemy can see the player or not.
- If the player has not seen, it calls FindClosestPoint function.

BTRotateToFindPlayer.h, BTRotateToFindPlayer.cpp
- When the loses the player or hears a noise, it goes to the place where the event occurs lastly, and turns around once to find player.

BTClearTrace.h, BTClearTrace.cpp, PlayerTrace.h, PlayerTrace.cpp
- It shows the place where the enemy is going.
- It is placed at the player's position when the enemy detected the player last time.
