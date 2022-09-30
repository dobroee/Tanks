// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GameStruct.generated.h"

UENUM(BluePrintType)
enum class ECannonType : uint8
{
	FireProjectile = 0  UMETA(DisplayName = "Use projectile"),
	FireTrace = 1  UMETA(DisplayName = "Use trace")
};

USTRUCT()
struct FDamageData
{
	GENERATED_BODY()

	UPROPERTY()
	float DamageValue;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* DamageMaker;
};

USTRUCT()
struct FScoreData
{
	GENERATED_BODY()

	UPROPERTY()
	float ScoreValue;

	UPROPERTY()
	AActor* Instigator;

	UPROPERTY()
	AActor* ScoreMaker;
};

UCLASS()
class TANKS_API UGameStruct : public UObject
{
	GENERATED_BODY()
	
};
