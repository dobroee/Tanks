// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannoMesh"));
	CannonMesh->SetupAttachment(sceneComp);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::Fire()
{
	if(!isReadyToFire())
	{
		return;
	}
	bReadyToFire = false;

	if (CannoType == ECannonType::FireProjectile)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile")));
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace")));
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::FireSpecial()
{
	if (!isReadyToFire())
	{
		return;
	}
	bReadyToFire = false;

	for (int i = 1; i <= NumberOfShellsInBurst; i++)
	{
		if (CannoType == ECannonType::FireProjectile)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire projectile (special attack) %i / %i"), i, NumberOfShellsInBurst));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Fire trace (special attack) %i / %i"), i, NumberOfShellsInBurst));
		}
		
		//GetWorld()->GetTimerManager().SetTimer(BurstReloadTimer, this, &ACannon::Burst, BurstRate, false);
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::Reload()
{
	bReadyToFire = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Emerald, FString::Printf(TEXT("RELOAD")));
}

void ACannon::Burst()
{
	
}

bool ACannon::isReadyToFire()
{
	return bReadyToFire;
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
}