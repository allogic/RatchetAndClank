#pragma once

#include "RacWeaponBase.h"

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "RacTPSCharacterBase.generated.h"

class UInputComponent;
class UCameraComponent;

class ARacWeaponBase;

UCLASS()
class RATCHETANDCLANK_API ARacTPSCharacterBase : public ACharacter
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Sub Components
  ////////////////////////////////////////////////////////////////////////

public:

  inline UCameraComponent* GetComponentCamera() const { return mComponentCamera; }

protected:

  UPROPERTY(EditAnywhere, Category = RacComponent)
    UCameraComponent* mComponentCamera = nullptr;

  ////////////////////////////////////////////////////////////////////////
  // Locomotion System
  ////////////////////////////////////////////////////////////////////////

public:

  UPROPERTY(EditAnywhere, Category = RacLocomotion)
    float mLocomotionSpeedWalkingForward = 200.0f;

  UPROPERTY(EditAnywhere, Category = RacLocomotion)
    float mLocomotionSpeedWalkingBack = 170.0f;

  UPROPERTY(EditAnywhere, Category = RacLocomotion)
    float mLocomotionSpeedRunningForward = 480.0f;

  UPROPERTY(EditAnywhere, Category = RacLocomotion)
    float mLocomotionSpeedRunningBack = 340.0f;

  ////////////////////////////////////////////////////////////////////////
  // Camera System
  ////////////////////////////////////////////////////////////////////////

public:

  inline bool GetCameraInvertYaw() const { return mCameraInvertYaw; }
  inline bool GetCameraInvertPitch() const { return mCameraInvertPitch; }

  inline float GetCameraPitchMin() const { return mCameraPitchMin; }
  inline float GetCameraPitchMax() const { return mCameraPitchMax; }

private:

  UPROPERTY(EditAnywhere, Category = RacCamera)
    bool mCameraInvertYaw = false;

  UPROPERTY(EditAnywhere, Category = RacCamera)
    bool mCameraInvertPitch = false;

  UPROPERTY(EditAnywhere, Category = RacCamera)
    float mCameraPitchMin = -45.0f;

  UPROPERTY(EditAnywhere, Category = RacCamera)
    float mCameraPitchMax = 45.0f;

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraPivotLocation = FVector{ 0.0f, 100.0f, 50.0f };

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraOffsetLocation = FVector{ -260.0f, 0.0f, 0.0f };

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraRandomRotationFrequency = FVector{ 0.8f, 0.8f, 0.8f };

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraRandomRotationIntensity = FVector{ 0.003f, 0.003f, 0.003f };

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraBoneRotationIntensity = FVector{ 0.05f, 0.05f, 0.05f };

  UPROPERTY(EditAnywhere, Category = RacCamera)
    FVector mCameraBoneRotationOffset = FVector{ 0.0f, 0.0f, -7.0f };

  ////////////////////////////////////////////////////////////////////////
  // Constructor
  ////////////////////////////////////////////////////////////////////////

public:

	ARacTPSCharacterBase();

  ////////////////////////////////////////////////////////////////////////
  // Character Overrides
  ////////////////////////////////////////////////////////////////////////

protected:

  virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

  ////////////////////////////////////////////////////////////////////////
  // Locomotion System
  ////////////////////////////////////////////////////////////////////////

public:

  inline void SetLocomotionRunning(bool Value) { mLocomotionIsRunning = Value; }
  inline void SetLocomotionJumping(bool Value) { bPressedJump = Value; }

private:

  bool mLocomotionIsRunning = false;

  void UpdateLocomotion(float DeltaTime);

  ////////////////////////////////////////////////////////////////////////
  // Weapon System
  ////////////////////////////////////////////////////////////////////////

public:

  inline void SetWeaponAiming(bool Value) { mWeaponIsAiming = Value; }
  inline void SetWeaponFiring(bool Value) { if (mWeapons[mWeaponCurrent]) mWeapons[mWeaponCurrent]->SetFiring(Value); }

  void TriggerWeaponSwap();

private:

  ARacWeaponBase* mWeapons[3] = {};

  int mWeaponCurrent = 0;

  bool mWeaponIsAiming = false;

  ////////////////////////////////////////////////////////////////////////
  // Camera System
  ////////////////////////////////////////////////////////////////////////

private:

  void UpdateCameraLocation();
  void UpdateCameraRotation();

  ////////////////////////////////////////////////////////////////////////
  // IK System
  ////////////////////////////////////////////////////////////////////////

private:

  void UpdateHandIK(float DeltaTime);
  void UpdateSpineIK(float DeltaTime);
};