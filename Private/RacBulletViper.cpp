#include "RacBulletViper.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

ARacBulletViper::ARacBulletViper()
{
  static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraSystemBeam(TEXT("NiagaraSystem'/Game/RatchetAndClank/Effects/ViperBeam_NS.ViperBeam_NS'"));

  if (niagaraSystemBeam.Succeeded()) mNiagaraBeam = niagaraSystemBeam.Object;
}

void ARacBulletViper::BeginPlay()
{
  Super::BeginPlay();

  UNiagaraFunctionLibrary::SpawnSystemAttached(mNiagaraBeam, RootComponent, TEXT(""), FVector::ZeroVector, FRotator::ZeroRotator, FVector::OneVector, EAttachLocation::SnapToTarget, false, ENCPoolMethod::None, true, false);
}

void ARacBulletViper::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  SetActorLocation(GetActorLocation() + GetActorForwardVector() * DeltaTime * 5000.0f, true);
}