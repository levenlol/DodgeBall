// Fill out your copyright notice in the Description page of Project Settings.


#include "PawnPositionObservation.h"
#include "../Core/BaseBallPawn.h"
#include "Kismet/GameplayStatics.h"

void UPawnPositionObservation::Init()
{
	BallPawn = Cast<ABaseBallPawn>(UGameplayStatics::GetActorOfClass(this, ABaseBallPawn::StaticClass()));
}

TArray<float> UPawnPositionObservation::ComputeObservation() const
{
	const FVector PawnLocation = BallPawn->GetActorLocation();
	
	const float X = FMath::GetMappedRangeValueUnclamped(LongitudinalLimits, FVector2D(-1.f, 1.f), PawnLocation.X);
	const float Y = FMath::GetMappedRangeValueUnclamped(LateralLimits, FVector2D(-1.f, 1.f), PawnLocation.Y);
	
	GEngine->AddOnScreenDebugMessage(489, -1.f, FColor::Blue, FString::Printf(TEXT("PawnObservationNormalization: X=%f, Y=%f"), X, Y));

	return { X, Y };
}
