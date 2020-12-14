//------------------------------------------------------------------------------
//
// File Name:    EnemyCharacter.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright © 2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#include "EnemyCharacter.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

#include "EngineUtils.h"
#include "UObject/ConstructorHelpers.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "EnemyAIController.h"

#define print(text) if(GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, text);

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BaseMaterial"));
	SeeingMaterial = CreateDefaultSubobject<UMaterial>(TEXT("SeeingMaterial"));

	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensingComp->SetPeripheralVisionAngle(90.f);

	//UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	//CapsuleComp->SetSimulatePhysics(true);
	//CapsuleComp->SetNotifyRigidBodyCollision(true);
	//CapsuleComp->BodyInstance.SetCollisionProfileName("BlockAllDynamic");
	//CapsuleComp->OnComponentHit.AddDynamic(this, &AEnemyCharacter::OnComponentHit);

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetSimulatePhysics(true);
	SphereComponent->SetCollisionProfileName(TEXT("Trigger"));
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacter::OnOverlapBegin);
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<UStaticMeshComponent*> StaticMeshComps;
	GetComponents<UStaticMeshComponent>(StaticMeshComps);

	for (auto& mesh : StaticMeshComps)
	{
		if (mesh->GetName().Contains("LeftEye"))
			LeftEye = mesh;
		else if (mesh->GetName().Contains("RightEye"))
			RightEye = mesh;
	}

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnPlayerSeen);
	}
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::SetBaseMaterial()
{
	LeftEye->SetMaterial(0, BaseMaterial);
	RightEye->SetMaterial(0, BaseMaterial);
}

void AEnemyCharacter::OnPlayerSeen(APawn* InPawn)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController)
	{
		//print("You have been seen");
		EnemyAIController->SetPlayerSeen(InPawn);

		LeftEye->SetMaterial(0, SeeingMaterial);
		RightEye->SetMaterial(0, SeeingMaterial);
	}
}

void AEnemyCharacter::OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//print("overlap");
	if (OtherActor && OtherActor != this && OtherComp)
	{
		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

		if (EnemyAIController)
		{
			EnemyAIController->OnEnemyHit(OtherActor);
		}
	}
}

//void AEnemyCharacter::OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
//{
//	//print("hit");
//	if (OtherActor && OtherActor != this && OtherComp)
//	{
//		AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
//
//		if (EnemyAIController)
//		{
//			EnemyAIController->OnEnemyHit(OtherActor);
//		}
//	}
//}