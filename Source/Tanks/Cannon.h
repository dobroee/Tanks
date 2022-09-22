// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameStruct.h"
#include "Cannon.generated.h"

UCLASS()
class TANKS_API ACannon : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannon();

	void Fire();
	void FireSpecial();
	void Reload();
	void Burst();
	bool isReadyToFire();

	FTimerHandle ReloadTimer;
	FTimerHandle BurstReloadTimer;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UStaticMeshComponent* CannonMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		class UArrowComponent* ProjectileSpawnPoint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
		ECannonType CannoType = ECannonType::FireProjectile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		float FireDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params")
		int NumberOfShellsInTank = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params|Burst")
		int NumberOfShellsInBurst = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Fire params|Burst")
		float BurstRate = 1.0f;

private:
	bool bReadyToFire = false;
};
