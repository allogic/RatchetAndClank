#pragma once

#include "CoreMinimal.h"

#include "RacBulletBase.h"

#include "RacBulletRyno.generated.h"

class UStaticMeshComponent;

class UNiagaraSystem;

UCLASS()
class RATCHETANDCLANK_API ARacBulletRyno : public ARacBulletBase
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Sub Components
  ////////////////////////////////////////////////////////////////////////

protected:

  UPROPERTY(EditAnywhere, Category = RacComponent)
    UStaticMeshComponent* mComponentBullet = nullptr;

  ////////////////////////////////////////////////////////////////////////
  // Constructor
  ////////////////////////////////////////////////////////////////////////

public:

  ARacBulletRyno();

  ////////////////////////////////////////////////////////////////////////
  // Actor Overrides
  ////////////////////////////////////////////////////////////////////////

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  ////////////////////////////////////////////////////////////////////////
  // Bullet System
  ////////////////////////////////////////////////////////////////////////

private:

  float mCurrentPitch = 0.0f;
  float mCurrentYaw = 0.0f;

  ////////////////////////////////////////////////////////////////////////
  // Effect System
  ////////////////////////////////////////////////////////////////////////

private:

  UNiagaraSystem* mNiagaraSmoke = nullptr;
};