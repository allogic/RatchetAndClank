#include "RacTPSCharacterRatchet.h"

#include "Engine/SkeletalMesh.h"

#include "Animation/AnimInstance.h"

ARacTPSCharacterRatchet::ARacTPSCharacterRatchet()
{
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshRatchet(TEXT("SkeletalMesh'/Game/RatchetAndClank/Meshes/Characters/Ratchet/Ratchet_SM.Ratchet_SM'"));

  static ConstructorHelpers::FClassFinder<UAnimInstance> animationInstanceRatchet(TEXT("/Game/RatchetAndClank/Animation/Ratchet/Ratchet_ABP"));

  if (skeletalMeshRatchet.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMeshRatchet.Object);

  if (animationInstanceRatchet.Succeeded()) GetMesh()->SetAnimClass(animationInstanceRatchet.Class);
}