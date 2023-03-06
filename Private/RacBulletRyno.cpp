#include "RacBulletRyno.h"

#include "Components/StaticMeshComponent.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ARacBulletRyno::ARacBulletRyno()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshMissile(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Weapons/Ryno/Missile_SM.Missile_SM'"));

  static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraSystemSmoke(TEXT("NiagaraSystem'/Game/RatchetAndClank/Effects/RynoMissile_NS.RynoMissile_NS'"));

  mComponentBullet = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bullet"));
  mComponentBullet->SetupAttachment(RootComponent);
  mComponentBullet->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  if (staticMeshMissile.Succeeded()) mComponentBullet->SetStaticMesh(staticMeshMissile.Object);

  if (niagaraSystemSmoke.Succeeded()) mNiagaraSmoke = niagaraSystemSmoke.Object;
}

void ARacBulletRyno::BeginPlay()
{
  Super::BeginPlay();

  UNiagaraFunctionLibrary::SpawnSystemAttached(mNiagaraSmoke, RootComponent, TEXT(""), FVector::ZeroVector, FRotator::ZeroRotator, FVector::OneVector, EAttachLocation::SnapToTarget, false, ENCPoolMethod::None, true, false);
}

void ARacBulletRyno::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  mCurrentPitch += FMath::RandRange(-1.0f, 1.0f) * DeltaTime * 10.0f;
  mCurrentYaw += FMath::RandRange(-1.0f, 1.0f) * DeltaTime * 10.0f;

  SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 2000.0f, true);
  SetActorRotation(GetActorRotation().Quaternion() * FRotator{ mCurrentPitch, mCurrentYaw, 0.0f }.Quaternion());
}