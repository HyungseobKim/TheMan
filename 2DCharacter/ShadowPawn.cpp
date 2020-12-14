//------------------------------------------------------------------------------
//
// File Name:    ShadowPawn.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------


#include "ShadowPawn.h"

#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"

#include "EngineUtils.h"

#include "ShadowPawnMovementComponent.h"
#include "WallComponent.h"
#include "FloorComponent.h"

#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, text);
#define printFString(text, fstring) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT(text), fstring));

// Sets default values
AShadowPawn::AShadowPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Root component is a box that reacts to physics
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	RootComponent = BoxComponent;
	BoxComponent->InitBoxExtent(FVector(125.f, 125.f, 125.f));
	BoxComponent->SetCollisionProfileName(TEXT("ShadowPawn"));

	// Register events.
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AShadowPawn::OnOverlapBegin);
	BoxComponent->OnComponentEndOverlap.AddDynamic(this, &AShadowPawn::OnOverlapEnd);
	
	// Decal component used as visual.
	UDecalComponent* DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(RootComponent);
	//DecalComponent->SetRelativeLocation(FVector(0.f, 0.f, -100.f));
	DecalComponent->SetWorldScale3D(FVector(1.f, 0.5f, 0.5f));

	// Use a spring arm to give the camera smooth, natural-feeling motion.
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraAttachmentArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	SpringArm->TargetArmLength = 500.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	// Create a camera and attach to our spring arm
	UCameraComponent* Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActualCamera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);

	// Take control of the default player
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	// Create an instance of our movement component, and tell it to update the root.
	ShadowMovementComponent = CreateDefaultSubobject<UShadowPawnMovementComponent>(TEXT("ShadowMovementComponent"));
	ShadowMovementComponent->UpdatedComponent = RootComponent;
}

UPawnMovementComponent* AShadowPawn::GetMovementComponent() const
{
	return ShadowMovementComponent;
}

void AShadowPawn::MoveForward(float AxisValue)
{
	if (ShadowMovementComponent && (ShadowMovementComponent->UpdatedComponent == RootComponent))
	{
		if (!onWall)
			ShadowMovementComponent->AddInputVector(GetActorForwardVector() * AxisValue);
		else
			ShadowMovementComponent->AddInputVector(GetActorUpVector() * AxisValue);
	}
}

void AShadowPawn::MoveRight(float AxisValue)
{
	if (ShadowMovementComponent && (ShadowMovementComponent->UpdatedComponent == RootComponent))
	{
		ShadowMovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}
}

void AShadowPawn::Turn(float AxisValue)
{
	if (onWall)
		SpringArm->AddRelativeRotation(FRotator(0.f, AxisValue, 0.f));
	else
	{
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += AxisValue;
		SetActorRotation(NewRotation);
	}
}

void AShadowPawn::LookUp(float AxisValue)
{
	SpringArm->AddRelativeRotation(FRotator(AxisValue, 0.f, 0.f));
}

void AShadowPawn::Switch()
{
	// Maybe there is better way
	ACharacter* player = nullptr;
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		player = Cast<ACharacter>(*ActorItr);

		if (player)
		{
			if (player->GetName().Contains(FString("ThirdPerson")))
			{
				AController* controller = GetController();

				if (controller)
				{
					controller->Possess(player);
					Destroy();
				}
			}
		}
	}
}

void AShadowPawn::OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromWeep, const FHitResult& SweepResult)
{
	/*print("Overalp Begin");
	if (OverlappedComp)
		printFString("Overlapped Actor: %s", *OtherActor->GetName());*/

	if (OtherActor && OtherActor->FindComponentByClass<UWallComponent>())
	{
		onWall = true;

		auto rotation = GetOwner()->GetActorRotation();
		auto wall = OtherActor->GetActorRotation();
		rotation.Yaw = OtherActor->GetActorRotation().Yaw;

		printFString("Yaw: %f", FMath::RadiansToDegrees(wall.Yaw));
		printFString("Pitch: %f", FMath::RadiansToDegrees(wall.Pitch));
		printFString("Roll: %f", FMath::RadiansToDegrees(wall.Roll));

		auto direction = ShadowMovementComponent->GetLastInputVector();
		if (FVector::DotProduct(OtherActor->GetActorForwardVector(), direction) < 0.f)
			rotation.Yaw += 180.f;

		//RootComponent->SetWorldLocation();
		RootComponent->MoveComponent(direction * 70.f, rotation, false);

		print("Overalp Begin");
		printFString("Overlapped Actor: %s", *OtherActor->GetName());
	}
	else if (OtherActor && OtherActor->FindComponentByClass<UFloorComponent>())
	{
		onWall = false;
		print("Overalp Begin");
		printFString("Overlapped Actor: %s", *OtherActor->GetName());
	}

	/*if (OtherActor->GetName().Contains(FString("Floor")))
	{
		onWall = false;
		print("Overalp Begin");
		printFString("Overlapped Actor: %s", *OtherActor->GetName());
	}
	else
	{
		onWall = true;
		RootComponent->SetWorldLocation(RootComponent->GetComponentLocation()+FVector(50.f, 0.f, 0.f));
	}*/
}

void AShadowPawn::OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	/*print("Overalp Ended");
	if (OtherActor && (OtherActor != this))
		printFString("Overlapped Actor: %s", *OtherActor->GetName());*/

	if (OtherActor && OtherActor->FindComponentByClass<UWallComponent>())
	{
		print("Overalp Ended");
		printFString("Overlapped Actor: %s", *OtherActor->GetName());

		onWall = false;
		SpringArm->SetRelativeRotation(FRotator(-45.f, 0.f, 0.f));
	}
}

// Called when the game starts or when spawned
void AShadowPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShadowPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShadowPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AShadowPawn::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AShadowPawn::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &AShadowPawn::Turn);
	PlayerInputComponent->BindAxis("LookUp", this, &AShadowPawn::LookUp);

	PlayerInputComponent->BindAction("Switch", IE_Pressed, this, &AShadowPawn::Switch);
}

