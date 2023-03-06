#include "RacCrateMetal.h"

#include "Engine/StaticMesh.h"

#include "Sound/SoundCue.h"

ARacCrateMetal::ARacCrateMetal()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshCrate(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Crates/Metal/Metal_SM.Metal_SM'"));

  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueExplosion(TEXT("SoundCue'/Game/RatchetAndClank/Sounds/Crates/Metal_SC.Metal_SC'"));

  if (staticMeshCrate.Succeeded()) mComponentMesh->SetStaticMesh(staticMeshCrate.Object);

  if (soundCueExplosion.Succeeded()) mSoundExplosion = soundCueExplosion.Object;
}