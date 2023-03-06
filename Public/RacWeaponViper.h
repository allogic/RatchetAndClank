#pragma once

#include "CoreMinimal.h"

#include "RacWeaponBase.h"

#include "RacWeaponViper.generated.h"

class UStaticMeshComponent;
class USceneComponent;

class UNiagaraSystem;

UCLASS()
class RATCHETANDCLANK_API ARacWeaponViper : public ARacWeaponBase
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Sub Components
	////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentWeaponLeft = nullptr;

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentWeaponRight = nullptr;

	UPROPERTY(EditAnywhere, Category = RacComponent)
		USceneComponent* mBulletSpawnLocations[2] = {};

	UPROPERTY(EditAnywhere, Category = RacComponent)
		USceneComponent* mShellSpawnLocations[2] = {};

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacWeaponViper();

	////////////////////////////////////////////////////////////////////////
	// Weapon System
	////////////////////////////////////////////////////////////////////////

public:

	virtual void AttachToCharacter(ARacTPSCharacterBase* Character) override;

protected:

	virtual void Fire() override;

private:

	int mActiveWeaponIndex = 0;

	float mRecoilIntensity = 1.0f;
	float mRecoilLeft = 0.0f;
	float mRecoilRight = 0.0f;

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

	////////////////////////////////////////////////////////////////////////
	// Effect System
	////////////////////////////////////////////////////////////////////////

private:

	UNiagaraSystem* mNiagaraShell = nullptr;
};