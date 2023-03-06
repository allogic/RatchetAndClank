#include "RacWeaponBase.h"

#include "Components/SphereComponent.h"

ARacWeaponBase::ARacWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void ARacWeaponBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARacWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARacWeaponBase::ActorDisable()
{
	SetFiring(false);

	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ARacWeaponBase::ActorEnable()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

void ARacWeaponBase::SetFiring(bool Value)
{
	if (Value)
	{
		GetWorldTimerManager().SetTimer(mFiringTimer, this, &ARacWeaponBase::Fire, mFireRate, true, 0.0f);
	}
	else
	{
		GetWorldTimerManager().ClearTimer(mFiringTimer);
	}
}