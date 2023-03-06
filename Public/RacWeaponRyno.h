#pragma once

#include "CoreMinimal.h"

#include "RacWeaponBase.h"

#include "RacWeaponRyno.generated.h"

class UStaticMeshComponent;
class USceneComponent;

UCLASS()
class RATCHETANDCLANK_API ARacWeaponRyno : public ARacWeaponBase
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Sub Components
	////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentWeapon = nullptr;

	UPROPERTY(EditAnywhere, Category = RacComponent)
		USceneComponent* mBulletSpawnLocations[8] = {};

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacWeaponRyno();

	////////////////////////////////////////////////////////////////////////
	// Weapon System
	////////////////////////////////////////////////////////////////////////

public:

	virtual void AttachToCharacter(ARacTPSCharacterBase* Character) override;

protected:

	virtual void Fire() override;

private:

	int mBulletSpawnIndex = 0;

	float mRecoilIntensity = 1.0f;
	float mRecoil = 0.0f;

	////////////////////////////////////////////////////////////////////////
	// IK System
	////////////////////////////////////////////////////////////////////////

public:

	virtual bool IsUsingLeftHand() const override { return true; }
	virtual bool IsUsingRightHand() const override { return true; }

	virtual FTransform GetLeftHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime);
	virtual FTransform GetRightHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime);

	virtual FTransform GetLeftHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime);
	virtual FTransform GetRightHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime);
};