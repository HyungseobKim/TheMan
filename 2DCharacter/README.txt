2D character can move along the wall and floor in 3D game.

ShadowPawn.h, ShadowPawn.cpp
Character class. Used decal component as visual, so looks like shadow.
When character hit the wall, change movement along x-axis into z-axis, so can move along the wall.
When character hit the floor during moving along the wall, restore movement axis.

ShadowPawnMovementComponent.h, ShadowPawnMovementComponent.cpp
In unreal, character does not have capsule component must have custom movement component.

WallComponent.h, WallComponent.cpp
Represent movable space which is type wall.

FloorComponent.h, FloorComponent.cpp
Represent movable space which is type floor.