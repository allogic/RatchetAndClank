#pragma once

#include "CoreMinimal.h"

#include "RacBulletBase.h"

#include "RacBulletViper.generated.h"

class UNiagaraSystem;

UCLASS()
class RATCHETANDCLANK_API ARacBulletViper : public ARacBulletBase
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Constructor
  ////////////////////////////////////////////////////////////////////////

public:

  ARacBulletViper();

  ////////////////////////////////////////////////////////////////////////
  // Actor Overrides
  ////////////////////////////////////////////////////////////////////////

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;

  ////////////////////////////////////////////////////////////////////////
  // Effect System
  ////////////////////////////////////////////////////////////////////////

private:

  UNiagaraSystem* mNiagaraBeam = nullptr;
};