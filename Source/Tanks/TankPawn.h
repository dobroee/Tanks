// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class ACannon;

UCLASS()
class TANKS_API ATankPawn : public APawn
{
	GENERATED_BODY()

public:
	ATankPawn();

	virtual void Tick(float DeltaTime) override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void RotateRight(float Value);

	void SetupCannon();
	void Fire();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* BodyMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* TurretMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cannon")
		class UArrowComponent* CannonSetupPoint;

	UPROPERTY()
		ACannon* Cannon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
		float MovementSpeed = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
		float RotationSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
		float RotateInterpolationKey = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turret|Speed")
		float TurretInterpolationKey = 0.1f;

private:
	class ATankController* TankController;

	float ForwardMoveAxisValue = 0.0f;
	float RightMoveAxisValue = 0.0f;
	float RotateRightAxisValue = 0.0f;
	float CurrentRightAxixValue = 0.0f;
};
