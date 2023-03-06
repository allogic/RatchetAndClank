#include "RacTPSCharacterBase.h"
#include "RacTPSAnimInstance.h"
#include "RacMath.h"
#include "RacWeaponViper.h"
#include "RacWeaponRyno.h"
#include "RacCollisionChannels.h"

#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

ARacTPSCharacterBase::ARacTPSCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

  mComponentCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
  mComponentCamera->SetupAttachment(RootComponent);

  GetMesh()->SetRelativeLocation(FVector{ 0.0f, 0.0f, -88.0f });
  GetMesh()->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });

  GetCharacterMovement()->NavAgentProps.bCanJump = true;
  GetCharacterMovement()->NavAgentProps.bCanCrouch = true;
  GetCharacterMovement()->JumpZVelocity = 500.0f;
}

void ARacTPSCharacterBase::BeginPlay()
{
	Super::BeginPlay();

  // Setup weapons
  mWeapons[0] = nullptr;

  mWeapons[1] = GetWorld()->SpawnActor<ARacWeaponViper>(ARacWeaponViper::StaticClass());
  mWeapons[1]->AttachToCharacter(this);
  mWeapons[1]->ActorDisable();

  mWeapons[2] = GetWorld()->SpawnActor<ARacWeaponRyno>(ARacWeaponRyno::StaticClass());
  mWeapons[2]->AttachToCharacter(this);
  mWeapons[2]->ActorDisable();
}

void ARacTPSCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

  UpdateLocomotion(DeltaTime);

  UpdateCameraRotation();
  UpdateCameraLocation();

  UpdateHandIK(DeltaTime);
  UpdateSpineIK(DeltaTime);
}

void ARacTPSCharacterBase::UpdateLocomotion(float DeltaTime)
{
  FVector velocity = GetActorRotation().UnrotateVector(GetVelocity());

  bool isMovingForward = velocity.X >= 0.0f;

  float characterSpeed = 0.0f;
  float animationDirection = 0.0f;
  float animationSpeed = 0.0f;

  if (isMovingForward)
  {
    GetCharacterMovement()->MaxWalkSpeed = mLocomotionIsRunning ? mLocomotionSpeedRunningForward : mLocomotionSpeedWalkingForward;

    animationDirection = mLocomotionIsRunning
      ? FRacMath::Remap(velocity.Y, 0.0f, mLocomotionSpeedRunningForward, 0.0f, 1.0f)
      : FRacMath::Remap(velocity.Y, 0.0f, mLocomotionSpeedWalkingForward, 0.0f, 0.5f);

    animationSpeed = mLocomotionIsRunning
      ? FRacMath::Remap(velocity.X, mLocomotionSpeedWalkingForward, mLocomotionSpeedRunningForward, 0.5f, 1.0f)
      : FRacMath::Remap(velocity.X, 0.0f, mLocomotionSpeedWalkingForward, 0.0f, 0.5f);
  }
  else
  {
    GetCharacterMovement()->MaxWalkSpeed = mLocomotionIsRunning ? mLocomotionSpeedRunningBack : mLocomotionSpeedWalkingBack;

    animationDirection = mLocomotionIsRunning
      ? FRacMath::Remap(velocity.Y, 0.0f, mLocomotionSpeedRunningBack, 0.0f, 1.0f)
      : FRacMath::Remap(velocity.Y, 0.0f, mLocomotionSpeedWalkingBack, 0.0f, 0.5f);

    animationSpeed = mLocomotionIsRunning
      ? FRacMath::Remap(velocity.X, mLocomotionSpeedWalkingBack, mLocomotionSpeedRunningBack, 0.5f, 1.0f)
      : FRacMath::Remap(velocity.X, 0.0f, mLocomotionSpeedWalkingBack, 0.0f, 0.5f);
  }

  if (URacTPSAnimInstance* animInstance = Cast<URacTPSAnimInstance>(GetMesh()->GetAnimInstance()))
  {
    animInstance->mLocomotionDirection = FMath::Lerp(animInstance->mLocomotionDirection, animationDirection, DeltaTime * 15.0f);
    animInstance->mLocomotionSpeed = FMath::Lerp(animInstance->mLocomotionSpeed, animationSpeed, DeltaTime * 15.0f);
  }
}

void ARacTPSCharacterBase::TriggerWeaponSwap()
{
  if (mWeapons[mWeaponCurrent])
  {
    mWeapons[mWeaponCurrent]->ActorDisable();
  }

  mWeaponCurrent = (mWeaponCurrent + 1) % 3;

  if (mWeapons[mWeaponCurrent])
  {
    mWeapons[mWeaponCurrent]->ActorEnable();
  }
}

void ARacTPSCharacterBase::UpdateCameraLocation()
{
  FRotator cameraRotation = mComponentCamera->GetRelativeRotation();
  FVector cameraLocation = mCameraPivotLocation + cameraRotation.RotateVector(mCameraOffsetLocation);

  mComponentCamera->SetRelativeLocation(cameraLocation);
}

void ARacTPSCharacterBase::UpdateCameraRotation()
{
  float timeSinceStart = GetWorld()->GetRealTimeSeconds();

  float pitch = FMath::Sin(timeSinceStart * mCameraRandomRotationFrequency.X) * mCameraRandomRotationIntensity.X;
  float yaw = FMath::Cos(timeSinceStart * mCameraRandomRotationFrequency.Y) * mCameraRandomRotationIntensity.Y;
  float roll = (pitch + yaw) * mCameraRandomRotationIntensity.Z;

  mComponentCamera->AddRelativeRotation(FRotator{ pitch, yaw, roll });
}

void ARacTPSCharacterBase::UpdateHandIK(float DeltaTime)
{
  if (USkeletalMeshComponent* skeletalMesh = GetMesh())
  {
    if (URacTPSAnimInstance* animInstance = Cast<URacTPSAnimInstance>(skeletalMesh->GetAnimInstance()))
    {
      float cameraPitch = mComponentCamera->GetRelativeRotation().Pitch;

      float handLeftBlend = 0.0f;
      float handRightBlend = 0.0f;

      FTransform handLeftTransform = FTransform::Identity;
      FTransform handRightTransform = FTransform::Identity;

      if (mWeapons[mWeaponCurrent])
      {
        if (mWeapons[mWeaponCurrent]->IsUsingLeftHand())
        {
          handLeftBlend = mWeaponIsAiming ? 1.0f : 0.0f;
          handLeftTransform = mWeaponIsAiming
            ? mWeapons[mWeaponCurrent]->GetLeftHandAimTransform(skeletalMesh, cameraPitch, DeltaTime)
            : mWeapons[mWeaponCurrent]->GetLeftHandRestTransform(skeletalMesh, cameraPitch, DeltaTime);
        }

        if (mWeapons[mWeaponCurrent]->IsUsingRightHand())
        {
          handRightBlend = mWeaponIsAiming ? 1.0f : 0.0f;
          handRightTransform = mWeaponIsAiming
            ? mWeapons[mWeaponCurrent]->GetRightHandAimTransform(skeletalMesh, cameraPitch, DeltaTime)
            : mWeapons[mWeaponCurrent]->GetRightHandRestTransform(skeletalMesh, cameraPitch, DeltaTime);
        }
      }

      animInstance->mIKHandLeftBlend = FMath::Lerp(animInstance->mIKHandLeftBlend, handLeftBlend, DeltaTime * 10.0f);
      animInstance->mIKHandRightBlend = FMath::Lerp(animInstance->mIKHandRightBlend, handRightBlend, DeltaTime * 10.0f);
      animInstance->mIKHandLeftTransform = handLeftTransform;
      animInstance->mIKHandRightTransform = handRightTransform;
    }
  }
}

void ARacTPSCharacterBase::UpdateSpineIK(float DeltaTime)
{
  USkeletalMeshComponent* skeletalMesh = GetMesh();
  float cameraPitch = mComponentCamera->GetRelativeRotation().Pitch / 1.5f;

  if (URacTPSAnimInstance* animInstance = Cast<URacTPSAnimInstance>(skeletalMesh->GetAnimInstance()))
  {
    FVector pelvisLocation = skeletalMesh->GetBoneLocation(TEXT("pelvis"), EBoneSpaces::ComponentSpace);
    FVector spineLocation = FRotator{ 0.0f, 0.0f, -90.0f + -cameraPitch }.RotateVector(FVector{ 0.0f, 100.0f, 0.0f });

    animInstance->mIKSpineTransform = FTransform{ FRotator{ -cameraPitch, -90.0f, 90.0f }, pelvisLocation + spineLocation };
  }
}