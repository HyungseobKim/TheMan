//------------------------------------------------------------------------------
//
// File Name:    EnemyCharacter.h
// Author(s):    Hyungseob Kim (hyungseob.kim)
// Project:      The Man
// Course:       GAM300
//
// Copyright ?2020 DigiPen (USA) Corporation, all rights reserved.
//
//------------------------------------------------------------------------------

#pragma once

#include <limits>

#include "Components/SpotLightComponent.h"
#include "PlayerTrace.h"

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class GRIMGRIN_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/*UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);*/

	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetPlayerSeen(AActor* InActor);

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnHearNoise(APawn* PawnInstigator, const FVector& Location, float Volume);

	void SetBaseMaterial();

	void SpawnPlayerTrace(APawn* player);

	UFUNCTION(BlueprintImplementableEvent, Category = "BeastEvent")
	void SeeingPlayerEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "BeastEvent")
	void CantSeePlayerEvent();

	UFUNCTION(BlueprintImplementableEvent, Category = "BeastEvent")
	void CaughtPlayer();

	UFUNCTION(BlueprintCallable, Category = Eyes)
	void InitializeTimer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, Category = AI)
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class UPawnSensingComponent* PawnSensingComp;

	UPROPERTY(VisibleAnywhere, Category = AI)
	class USphereComponent* SphereComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float ChasingTime = 2.f;

	/*UPROPERTY(EditAnywhere, Category = Eyes)
	class UMaterial* BaseMaterial;

	UPROPERTY(EditAnywhere, Category = Eyes)
	class UMaterial* SeeingMaterial;*/

	UPROPERTY(EditAnywhere, Category = Eyes)
	FLinearColor EyeLightColorDefault;

	UPROPERTY(EditAnywhere, Category = Eyes)
	FLinearColor EyeLightColorSeen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Eyes)
	float ColorChangingTime = 2.f;

private:
	UFUNCTION()
	void OnPlayerSeen(APawn* InPawn);

	/*UStaticMeshComponent* LeftEye;
	UStaticMeshComponent* RightEye;*/

	USpotLightComponent* EyeLight;

	UPROPERTY(EditDefaultsOnly, Category = AI)
	TSubclassOf<APlayerTrace> PlayerTraceBP;

	APlayerTrace* playerTrace = nullptr;

	float EyeColorTimer = std::numeric_limits<float>::max();
};
