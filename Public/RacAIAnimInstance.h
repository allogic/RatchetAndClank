#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimInstance.h"

#include "RacAIAnimInstance.generated.h"

UCLASS()
class RATCHETANDCLANK_API URacAIAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Locomotion System
  ////////////////////////////////////////////////////////////////////////

public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacLocomotion)
    float mLocomotionDirection;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacLocomotion)
    float mLocomotionSpeed;

  ////////////////////////////////////////////////////////////////////////
  // IK System
  ////////////////////////////////////////////////////////////////////////

public:

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacIK)
    float mIKHandLeftBlend = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacIK)
    float mIKHandRightBlend = 0.0f;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacIK)
    FTransform mIKHandLeftTransform = FTransform::Identity;

  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RacIK)
    FTransform mIKHandRightTransform = FTransform::Identity;
};
