// Fill out your copyright notice in the Description page of Project Settings.


#include "CannonBox.h"
#include "TankPawn.h"

ACannonBox::ACannonBox()
{
	PrimaryActorTick.bCanEverTick = false;

	USceneComponent* SceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComp;

	CannonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CannonMesh"));
	CannonMesh->SetupAttachment(SceneComp);
	CannonMesh->OnComponentBeginOverlap.AddDynamic(this, &ACannonBox::OnMeshOverlapBegin);
	CannonMesh->SetCollisionProfileName("OverlapAll");
	CannonMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CannonMesh->SetGenerateOverlapEvents(true);
}

void ACannonBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ATankPawn* TankPawn = Cast<ATankPawn>(OtherActor);
	if (TankPawn)
	{
		TankPawn->SetupCannon(CannonClass);
	}
	Destroy();
}

