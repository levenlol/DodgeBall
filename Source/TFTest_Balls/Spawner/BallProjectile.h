// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BallProjectile.generated.h"

class USphereComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnProjectileCollisionDelegate, ABallProjectile*, ThisProjectile, AActor*, OtherActor);

UCLASS()
class TFTEST_BALLS_API ABallProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallProjectile();

	void Init(const FVector& Location, const FVector& Speed);
	void Uninit();

	bool IsActive() const;

	FOnProjectileCollisionDelegate OnCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = Setup)
	FVector ProjectileSpeed = FVector::ZeroVector;

	UPROPERTY(VisibleAnywhere, Category = Setup)
	USphereComponent* SphereCollider;


	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(VisibleAnywhere, Category = Debug)
	bool bIsActive = true;

	UPROPERTY(VisibleAnywhere, Category = Debug)
	FVector InitialLocation;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
