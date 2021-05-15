// Fill out your copyright notice in the Description page of Project Settings.


#include "BallSpawner.h"
#include "BallProjectile.h"

// Sets default values
ABallSpawner::ABallSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABallSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	UWorld* World = GetWorld();
	const FVector Location = GetActorLocation();

	for (int32 i = 0; i < BallsNum; i++)
	{
		ABallProjectile* Ball = World->SpawnActor<ABallProjectile>(BallProjectileClass, SpawnParams);
		Ball->Init(Location, GetRandomBallSpeed());
		Ball->OnCollision.AddDynamic(this, &ABallSpawner::OnProjectileCollide);

		BallProjectiles.Add(Ball);
	}
}

// Called every frame
void ABallSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	for (ABallProjectile* Projectile : BallProjectiles)
	{
		if (!Projectile->IsActive())
		{
			Projectile->Init(GetActorLocation(), GetRandomBallSpeed());
		}
	}

}

void ABallSpawner::OnProjectileCollide(ABallProjectile* Projectile, AActor* HitActor)
{
	Projectile->Uninit();
}

FVector ABallSpawner::GetRandomBallSpeed() const
{
	const float XSpeed = FMath::RandRange(-1.f, 1.f);
	const float YSpeed = FMath::RandRange(-1.f, 1.f);

	const FVector RandomSpeed = FVector(XSpeed, YSpeed, 0.f).GetClampedToMaxSize(1.f);
	return RandomSpeed * MaxBallSpeed;
}

