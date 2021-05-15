// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "BaseBallPawn.generated.h"

class UCapsuleComponent;



UCLASS()
class TFTEST_BALLS_API ABaseBallPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ABaseBallPawn();

	void SetMovementUp(const float AxisValue);
	void SetMovementRight(const float AxisValue);

	UPROPERTY(EditAnywhere, Category = Setup)
	float Speed = 1000.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Setup)
	UCapsuleComponent* Capsule;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	float MoveUpValue = 0.f;
	float MoveRightValue = 0.f;

};
