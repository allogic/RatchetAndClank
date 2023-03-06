#pragma once

#include "CoreMinimal.h"

#include "MassEntityTypes.h"

#include "RacMassFragments.generated.h"

USTRUCT()
struct RATCHETANDCLANK_API FRacMassFragmentFollower : public FMassFragment
{
	GENERATED_BODY()

	bool IsFollowing;

	float Speed;

	FVector Direction;
	FVector Target;
};