// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseBallPawn.h"

// Sets default values
ABaseBallPawn::ABaseBallPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseBallPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(FName("MoveUp"), this, &ABaseBallPawn::SetMovementUp);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ABaseBallPawn::SetMovementRight);
}

