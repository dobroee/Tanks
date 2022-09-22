// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankController.generated.h"

/**
 * 
 */
UCLASS()
class TANKS_API ATankController : public APlayerController
{
	GENERATED_BODY()

public:
	ATankController();
	virtual void SetPawn(APawn* InPawn) override;

	FVector GetMousePos() { return MousePos; };

protected:
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:
	class ATankPawn* TankPawn;

	FVector MousePos;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void Fire();
	void FireSpecial();
};