// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallPawn.h"
#include "Components/CapsuleComponent.h"

ABaseBallPawn::ABaseBallPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	RootComponent = Capsule;
}

void ABaseBallPawn::SetMovementUp(const float AxisValue)
{
	MoveUpValue = AxisValue;
}

void ABaseBallPawn::SetMovementRight(const float AxisValue)
{
	MoveRightValue = AxisValue;
}

// Called when the game starts or when spawned
void ABaseBallPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseBallPawn::Tick(float DeltaTime)
{
	// Clamp max size so that (X=1, Y=1) doesn't cause faster movement in diagonal directions
	const FVector MoveDirection = FVector(MoveUpValue, MoveRightValue, 0.f).GetClampedToMaxSize(1.0f);

	// Calculate  movement
	const FVector Movement = MoveDirection * Speed * DeltaTime;

	// If non-zero size, move this actor
	if (Movement.SizeSquared() > 0.0f)
	{
		//const FRotator NewRotation = Movement.Rotation();
		FHitResult Hit(1.f);
		RootComponent->MoveComponent(Movement, GetActorRotation(), true, &Hit);

		if (Hit.IsValidBlockingHit() && Hit.Actor->IsRootComponentStatic())
		{
			const FVector Normal2D = Hit.Normal.GetSafeNormal2D();
			const FVector Deflection = FVector::VectorPlaneProject(Movement, Normal2D) * (1.f - Hit.Time);
			RootComponent->MoveComponent(Deflection, GetActorRotation(), true);
		}
	}
}

// Called to bind functionality to input
void ABaseBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ABaseBallPawn::SetMovementUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseBallPawn::SetMovementRight);
}

