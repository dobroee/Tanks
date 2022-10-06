// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/BoxComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/StaticMeshComponent.h"
#include "Cannon.h"
#include "Components/ArrowComponent.h"
#include "HealthComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"

ATankPawn::ATankPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	RootComponent = BoxCollision;

	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BodyMesh"));
	BodyMesh->SetupAttachment(BoxCollision);

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretMesh"));
	TurretMesh->SetupAttachment(BodyMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(BoxCollision);
	SpringArm->bDoCollisionTest = false;
	SpringArm->bInheritPitch = false;
	SpringArm->bInheritRoll = false;
	SpringArm->bInheritYaw = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("CannonSetupPoint"));
	CannonSetupPoint->SetupAttachment(TurretMesh);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnHealthChanged.AddUObject(this, &ATankPawn::DamageTaked);

	DamageTakedSound = CreateDefaultSubobject<UAudioComponent>(TEXT("DamageTakedSound"));
	DamageTakedSound->SetAutoActivate(false);

	DamageTakedEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DamageTakedEffect"));
	DamageTakedEffect->SetAutoActivate(false);
	DamageTakedEffect->SetupAttachment(TurretMesh);
}

void ATankPawn::BeginPlay()
{
	Super::BeginPlay();

	TankController = Cast<ATankController>(GetController());
	SetupCannon(EquipedCannonClass);
}

void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//MoveForward, MoveRight
	FVector currentLocation = GetActorLocation();
	FVector ForwardVector = GetActorForwardVector();
	FVector RightVector = GetActorRightVector();
	FVector movePosition = currentLocation + ForwardVector * MovementSpeed * ForwardMoveAxisValue * DeltaTime + RightVector * MovementSpeed * RightMoveAxisValue * DeltaTime;
	SetActorLocation(movePosition);

	//BodyRotation
	CurrentRightAxixValue = FMath::Lerp(CurrentRightAxixValue, RotateRightAxisValue, RotateInterpolationKey);
	
	float yawRotation = CurrentRightAxixValue * RotationSpeed * DeltaTime;
	FRotator currentRotation = GetActorRotation();

	yawRotation += currentRotation.Yaw;
	FRotator newRotation = FRotator(0.0f, yawRotation, 0.0f);
	SetActorRotation(newRotation);

	//TurretRotation
	if (TankController)
	{
		FVector MousePos = TankController->GetMousePos();
		RotateTurretTo(MousePos);
	}
}

void ATankPawn::MoveForward(float Value)
{
	ForwardMoveAxisValue = Value;
}

void ATankPawn::MoveRight(float Value)
{
	RightMoveAxisValue = Value;
}

void ATankPawn::RotateRight(float Value)
{
	RotateRightAxisValue = Value;
}

void ATankPawn::SetupCannon(TSubclassOf<ACannon> newCannon)
{
	if (!newCannon)
	{
		return;
	}

	if (Cannon)
	{
		Cannon->Destroy();
	}

	EquipedCannonClass = newCannon;
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(newCannon, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

void ATankPawn::Fire()
{
	if (Cannon)
	{
		Cannon->Fire();
	}
}

void ATankPawn::FireSpecial()
{
	if (Cannon)
	{
		Cannon->FireSpecial();
	}
}

void ATankPawn::AddShells(int32 Shells)
{
	if (Cannon)
	{
		Cannon->AddShells(Shells);
	}
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator turretRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = turretRotation.Pitch;
	targetRotation.Roll = turretRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(turretRotation, targetRotation, TurretInterpolationKey));
}

FVector ATankPawn::GetEyesPosition() const
{
	return CannonSetupPoint->GetComponentLocation();
}


float ATankPawn::GetPoints()
{
	return ScoreValue;
}


void ATankPawn::ShowScore(float Value)
{
	Score += Value;
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, FString::Printf(TEXT("Score: %f"), Score));
}

void ATankPawn::Die()
{
	if (Cannon)
	{
		Cannon->Destroy();
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Tank %s is die, health: %f"), *GetName(), HealthComponent->GetHealth());

	DieEffect = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DieParticle, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), DieSound, GetActorLocation());

	Destroy();
}

void ATankPawn::DamageTaked(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("Tank %s taked damage: %f, health: %f"), *GetName(), Value, HealthComponent->GetHealth());

	DamageTakedEffect->ActivateSystem();
	DamageTakedSound->Play();
}


void ATankPawn::ChangeCannon()
{
	TSubclassOf<ACannon> CachedCanon;
	CachedCanon = EquipedCannonClass;
	EquipedCannonClass = SecondCannonClass;
	SecondCannonClass = CachedCanon;
	SetupCannon(EquipedCannonClass);
}
