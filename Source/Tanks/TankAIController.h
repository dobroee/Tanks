// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TankPawn.h"
#include "GameFramework/Pawn.h"
#include "TankAIController.generated.h"

UCLASS()
class TANKS_API ATankAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	class ATankPawn* TankPawn;

	UPROPERTY()
	APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIParams", Meta = (MakeEditWidget = true))
	TArray<FVector> PatrollingPoints;

	int32 PatrollingIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AIParams")
	float MovementAccurancy;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10.0f;

	virtual void Tick(float DeltaSeconds) override;

	float GetRotationValue();
	void Targeting();
	void RotateToPlayer();
	void Fire();
	bool IsPlayerInRange();
	bool CanFire();
	bool IsPlayerSeen();
};
