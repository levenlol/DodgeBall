// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallSpawner.generated.h"

class ABallProjectile;


UCLASS()
class TFTEST_BALLS_API ABallSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallSpawner();

	UPROPERTY(EditAnywhere, Category = Setup)
	int32 BallsNum = 30;

	UPROPERTY(EditAnywhere, Category = Setup)
	TSubclassOf<ABallProjectile> BallProjectileClass;

	UPROPERTY(EditAnywhere, Category = Setup)
	float MaxBallSpeed = 600.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere, Category = Debug)
	TArray<ABallProjectile*> BallProjectiles;

	UFUNCTION()
	void OnProjectileCollide(ABallProjectile* Projectile, AActor* HitActor);

	FVector GetRandomBallSpeed() const;

};
