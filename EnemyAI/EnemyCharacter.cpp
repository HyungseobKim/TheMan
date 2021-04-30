//------------------------------------------------------------------------------
//
// File Name:    EnemyCharacter.cpp
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright ?2020 DigiPen (USA) Corporation, all rights reserved.
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

	/*BaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BaseMaterial"));
	SeeingMaterial = CreateDefaultSubobject<UMaterial>(TEXT("SeeingMaterial"));*/

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

	/*TArray<UStaticMeshComponent*> StaticMeshComps;
	GetComponents<UStaticMeshComponent>(StaticMeshComps);

	for (auto& mesh : StaticMeshComps)
	{
		if (mesh->GetName().Contains("LeftEye"))
			LeftEye = mesh;
		else if (mesh->GetName().Contains("RightEye"))
			RightEye = mesh;
	}*/

	EyeLight = FindComponentByClass<USpotLightComponent>();
	EyeLight->SetLightColor(EyeLightColorDefault);

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AEnemyCharacter::OnPlayerSeen);
		PawnSensingComp->OnHearNoise.AddDynamic(this, &AEnemyCharacter::OnHearNoise);
	}

	InitializeTimer();

	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());
	if (EnemyAIController)
		EnemyAIController->SetChasingTime(ChasingTime);
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//print(FString::SanitizeFloat(EyeColorTimer));

	if (EyeColorTimer < ColorChangingTime)
	{
		EyeColorTimer += DeltaTime;

		// I can't use std::clamp.
		if (EyeColorTimer > ColorChangingTime)
			EyeColorTimer = ColorChangingTime;

		FLinearColor newColor = EyeLightColorDefault;
		const float amount = EyeColorTimer / ColorChangingTime;

		newColor.R += ((EyeLightColorSeen.R - EyeLightColorDefault.R) * amount);
		newColor.G += ((EyeLightColorSeen.G - EyeLightColorDefault.G) * amount);
		newColor.B += ((EyeLightColorSeen.B - EyeLightColorDefault.B) * amount);
		newColor.A += ((EyeLightColorSeen.A - EyeLightColorDefault.A) * amount);

		EyeLight->SetLightColor(newColor);
	}
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacter::SetBaseMaterial()
{
	/*LeftEye->SetMaterial(0, BaseMaterial);
	RightEye->SetMaterial(0, BaseMaterial);*/
	//print("Lose player");
	EyeColorTimer = ColorChangingTime + 1.f;
	EyeLight->SetLightColor(EyeLightColorDefault);
	CantSeePlayerEvent();
}

void AEnemyCharacter::InitializeTimer()
{
	EyeColorTimer = ColorChangingTime + 1.f;
}

void AEnemyCharacter::OnPlayerSeen(APawn* InPawn)
{
	AEnemyAIController* EnemyAIController = GetController<AEnemyAIController>();

	if (IsValid(EnemyAIController))
	{
		//print("You have been seen");
		EnemyAIController->SetPlayerSeen(InPawn);

		//EyeLight->SetLightColor(EyeLightColorSeen);
		if (EyeColorTimer == ColorChangingTime + 1.f)
			EyeColorTimer = 0.f;

		SeeingPlayerEvent();

		/*if (IsValid(playerTrace))
		{
			playerTrace->Destroy();
			playerTrace = nullptr;
		}*/
	}
}

void AEnemyCharacter::SetPlayerSeen(AActor* InActor)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController)
	{
		//print("You have been seen");
		EnemyAIController->SetPlayerSeen(InActor);

		//EyeLight->SetLightColor(EyeLightColorSeen);
		if (EyeColorTimer == ColorChangingTime + 1.f)
			EyeColorTimer = 0.f;

		/*if (IsValid(playerTrace))
		{
			playerTrace->Destroy();
			playerTrace = nullptr;
		}*/
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

void AEnemyCharacter::OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume)
{
	AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController());

	if (EnemyAIController && PawnInstigator != this)
	{
		EnemyAIController->SetPlayerHeard(PawnInstigator);

		//EyeLight->SetLightColor(EyeLightColorSeen);
		/*if (EyeColorTimer == ColorChangingTime + 1.f)
			EyeColorTimer = 0.f;*/
	}
}

void AEnemyCharacter::SpawnPlayerTrace(APawn* player)
{
	if (IsValid(player))
	{
		if (IsValid(playerTrace))
		{
			playerTrace->Destroy();
			playerTrace = nullptr;
		}

		FActorSpawnParameters SpawnParams;
		playerTrace = GetWorld()->SpawnActor<APlayerTrace>(PlayerTraceBP, player->GetTransform(), SpawnParams);
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