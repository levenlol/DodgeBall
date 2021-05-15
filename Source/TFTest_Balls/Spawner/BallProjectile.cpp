// Fill out your copyright notice in the Description page of Project Settings.


#include "BallProjectile.h"
#include "Components/SphereComponent.h"

// Sets default values
ABallProjectile::ABallProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = SphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collision");
}

void ABallProjectile::Init(const FVector& Location, const FVector& Speed)
{
	SetActorEnableCollision(true);
	SetActorHiddenInGame(false);
	SetActorTickEnabled(true);

	InitialLocation = Location;
	SetActorLocation(Location, false, nullptr, ETeleportType::ResetPhysics);

	ProjectileSpeed = Speed;
	bIsActive = true;
}

void ABallProjectile::Uninit()
{
	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
	SetActorTickEnabled(false);

	SetActorLocation(InitialLocation, false, nullptr, ETeleportType::ResetPhysics);

	ProjectileSpeed = FVector::ZeroVector;
	bIsActive = false;
}

bool ABallProjectile::IsActive() const
{
	return bIsActive;
}

// Called when the game starts or when spawned
void ABallProjectile::BeginPlay()
{
	Super::BeginPlay();

	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ABallProjectile::OnOverlap);
}

void ABallProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	SphereCollider->OnComponentBeginOverlap.RemoveDynamic(this, &ABallProjectile::OnOverlap);
}


void ABallProjectile::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor->IsA<ABallProjectile>())
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Magenta, FString("Collided"));
		OnCollision.Broadcast(this, OtherActor);
	}
}

// Called every frame
void ABallProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const FVector Location = GetActorLocation();

	SetActorLocation(Location + ProjectileSpeed * DeltaTime);
}

