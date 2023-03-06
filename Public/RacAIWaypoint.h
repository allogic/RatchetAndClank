#pragma once

#include "RacTeam.h"

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RacAIWaypoint.generated.h"

UCLASS()
class RATCHETANDCLANK_API ARacAIWaypoint : public AActor
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Team System
	////////////////////////////////////////////////////////////////////////

public:

	inline ERacTeam GetTeam() const { return mTeam; }

protected:

	UPROPERTY(EditAnywhere, Category = RacTeam)
		ERacTeam mTeam = ERacTeam::None;

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacAIWaypoint();
};