#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RacWeaponBase.generated.h"

class USkeletalMeshComponent;

class USoundCue;

class ARacTPSCharacterBase;

UCLASS()
class RATCHETANDCLANK_API ARacWeaponBase : public AActor
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacWeaponBase();

	////////////////////////////////////////////////////////////////////////
	// Character Overrides
	////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	////////////////////////////////////////////////////////////////////////
	// Weapon System
	////////////////////////////////////////////////////////////////////////

public:

	virtual void AttachToCharacter(ARacTPSCharacterBase* Character) {}

	virtual void ActorDisable();
	virtual void ActorEnable();

	virtual void SetFiring(bool Value);

protected:

	float mFireRate = 1.0f;

	FTimerHandle mFiringTimer = FTimerHandle{};

	virtual void Fire() {}

	////////////////////////////////////////////////////////////////////////
	// IK System
	////////////////////////////////////////////////////////////////////////

public:

	virtual bool IsUsingLeftHand() const { return false; };
	virtual bool IsUsingRightHand() const { return false; };

	virtual FTransform GetLeftHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime) { return FTransform::Identity; }
	virtual FTransform GetRightHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime) { return FTransform::Identity; }

	virtual FTransform GetLeftHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime) { return FTransform::Identity; }
	virtual FTransform GetRightHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime) { return FTransform::Identity; }

	////////////////////////////////////////////////////////////////////////
	// Sound System
	////////////////////////////////////////////////////////////////////////

protected:

	USoundCue* mSoundShot = nullptr;
};