// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "Projectile.h"
#include "DamageTaker.h"

ACannon::ACannon()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = sceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannoMesh"));
	CannonMesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnPoint"));
	ProjectileSpawnPoint->SetupAttachment(CannonMesh);
}

void ACannon::FireProjectile()
{
	if (ProjectilePool)
	{
		ProjectilePool->GetProjectile(ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	}
	else
	{
		FActorSpawnParameters spawnParams;
		spawnParams.Owner = this;
		AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
		if (projectile)
		{
			projectile->Start();
		}
	}
}

void ACannon::FireTrace()
{
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.bReturnPhysicalMaterial = false;

	FVector Start = ProjectileSpawnPoint->GetComponentLocation();
	FVector End = Start + ProjectileSpawnPoint->GetForwardVector() * FireRange;

	if (GetWorld()->LineTraceSingleByChannel(hitResult, Start, End, ECollisionChannel::ECC_Visibility, traceParams))
	{
		DrawDebugLine(GetWorld(), Start, hitResult.Location, FColor::Cyan, false, 1.0f, 0, 15.0f);
		IDamageTaker* DamageTakerActor = Cast<IDamageTaker>(hitResult.GetActor());
		if (hitResult.GetActor())
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = GetOwner();
			damageData.DamageMaker = this;

			DamageTakerActor->TakeDamage(damageData);
		}
		else
		{
			hitResult.GetActor()->Destroy();
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, false, 1.0f, 0, 15.0f);
	}
}

void ACannon::CreateProjectilePool()
{
	if (ProjectilePoolClass)
	{
		ProjectilePool = GetWorld()->SpawnActor<AProjectilePool>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
	}
}

void ACannon::Fire()
{
	if (NumberOfShellsInTank == 0)
	{
		PrintNoShells();
	}

	if(!isReadyToFire() || NumberOfShellsInTank <= 0)
	{
		return;
	}

	bReadyToFire = false;
	NumberOfShellsInTank--;

	if (CannonType == ECannonType::FireProjectile)
	{
		FireProjectile();
	}
	else
	{
		FireTrace();
	}

	GetWorld()->GetTimerManager().SetTimer(ReloadTimer, this, &ACannon::Reload, FireRate, false);
}

void ACannon::FireSpecial()
{
	if (NumberOfShellsInTank == 0)
	{
		PrintNoShells();
	}

	if (!isReadyToFire() || NumberOfShellsInTank <= 0)
	{
		return;
	}
	bReadyToFire = false;
	GetWorld()->GetTimerManager().SetTimer(BurstTimer, this, &ACannon::Burst, BurstRate, true, 0.0f);
}

void ACannon::Reload()
{
	bReadyToFire = true;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shells in tank - %i"), NumberOfShellsInTank));
}

bool ACannon::isReadyToFire()
{
	return bReadyToFire;
}

void ACannon::PrintNoShells()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("NO SHELLS")));
}

void ACannon::AddShells(int32 Shells)
{
	NumberOfShellsInTank += Shells;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Add shells %i, Total shells %i"), Shells, NumberOfShellsInTank));
}

void ACannon::Burst()
{
	if (CurrentBurst == NumberOfShellsInBurst)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstTimer);
		bReadyToFire = true;
		CurrentBurst = 0;
		NumberOfShellsInTank--;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Shells in tank - %i"), NumberOfShellsInTank));
		return;
	}

	CurrentBurst++;

	if (CannonType == ECannonType::FireProjectile)
	{
		FireProjectile();
	}
	else
	{
		FireTrace();
	}
}

void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();
	CreateProjectilePool();
}