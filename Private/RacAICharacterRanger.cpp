#include "RacAICharacterRanger.h"

#include "Engine/SkeletalMesh.h"

#include "Animation/AnimInstance.h"

ARacAICharacterRanger::ARacAICharacterRanger()
{
  static ConstructorHelpers::FObjectFinder<USkeletalMesh> skeletalMeshRanger(TEXT("SkeletalMesh'/Game/RatchetAndClank/Meshes/Characters/Ranger/Ranger_SM.Ranger_SM'"));

  static ConstructorHelpers::FClassFinder<UAnimInstance> animationInstanceRanger(TEXT("/Game/RatchetAndClank/Animation/Ranger/Ranger_ABP"));

  GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, -88.0f });
  GetMesh()->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });

  if (skeletalMeshRanger.Succeeded()) GetMesh()->SetSkeletalMesh(skeletalMeshRanger.Object);

  if (animationInstanceRanger.Succeeded()) GetMesh()->SetAnimClass(animationInstanceRanger.Class);
}