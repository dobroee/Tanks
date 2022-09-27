// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Projectile.generated.h"

UCLASS()
class TANKS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AProjectile();

	void Start();

	FTimerHandle MoveTimer;

protected:
	UFUNCTION()
	void Move();

	UFUNCTION()
	void OnMeshOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* ProjectileMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class USphereComponent* SphereCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveSpeed = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement|Speed")
	float MoveRate = 0.005f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Damage")
		float Damagee = 1.0f;
};