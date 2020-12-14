//------------------------------------------------------------------------------
//
// File Name:    WallComponent.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "WallComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GRIMGRIN_API UWallComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWallComponent();
};
