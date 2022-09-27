// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CannonBox.generated.h"

UCLASS()
class TANKS_API ACannonBox : public AActor
{
	GENERATED_BODY()
	
public:	
	ACannonBox();

protected:
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	class UStaticMeshComponent* CannonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component")
	TSubclassOf<class ACannon> CannonClass;
};
