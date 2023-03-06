#include "RacTPSCharacterThief.h"

#include "Engine/SkeletalMesh.h"

#include "Animation/AnimInstance.h"

ARacTPSCharacterThief::ARacTPSCharacterThief()
{
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshThief(TEXT("SkeletalMesh'/Game/RatchetAndClank/Meshes/Characters/Thief/Thief_SM.Thief_SM'"));

  static ConstructorHelpers::FClassFinder<UAnimInstance> animationInstanceThief(TEXT("/Game/RatchetAndClank/Animation/Thief/Thief_ABP"));

  if (skeletalMeshThief.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMeshThief.Object);

  if (animationInstanceThief.Succeeded()) GetMesh()->SetAnimClass(animationInstanceThief.Class);
}