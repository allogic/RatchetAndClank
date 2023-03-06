#include "RacBulletBase.h"
#include "RacCollisionChannels.h"

#include "Components/SphereComponent.h"

ARacBulletBase::ARacBulletBase()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

  mComponentCollider = CreateDefaultSubobject<USphereComponent>(TEXT("Collider"));
  mComponentCollider->SetupAttachment(RootComponent);
  mComponentCollider->SetSphereRadius(10.0f);
  mComponentCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
  mComponentCollider->SetCollisionObjectType(ERacCollisionChannel::ECC_Bullet);
  mComponentCollider->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void ARacBulletBase::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(mDestroyTimer, this, &ARacBulletBase::SelfDestruct, 5.0f, false);
}

void ARacBulletBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);
}

void ARacBulletBase::SelfDestruct()
{
	Destroy();
}