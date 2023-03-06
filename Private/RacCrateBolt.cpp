#include "RacCrateBolt.h"

#include "Engine/StaticMesh.h"

#include "Sound/SoundCue.h"

ARacCrateBolt::ARacCrateBolt()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshCrate(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Crates/Bolt/Bolt_SM.Bolt_SM'"));

  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueExplosion(TEXT("SoundCue'/Game/RatchetAndClank/Sounds/Crates/Bolt_SC.Bolt_SC'"));

  if (staticMeshCrate.Succeeded()) mComponentMesh->SetStaticMesh(staticMeshCrate.Object);

  if (soundCueExplosion.Succeeded()) mSoundExplosion = soundCueExplosion.Object;
}