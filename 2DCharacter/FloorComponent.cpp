//------------------------------------------------------------------------------
//
// File Name:    FloorComponent.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------


#include "FloorComponent.h"

#include "Components/StaticMeshComponent.h"

// Sets default values for this component's properties
UFloorComponent::UFloorComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// Turn on generating overlap events
	if (GetOwner())
	{
		UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();

		if (StaticMeshComponent)
			StaticMeshComponent->SetGenerateOverlapEvents(true);
	}
}