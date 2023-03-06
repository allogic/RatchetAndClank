#include "RacAIWaypoint.h"

#include "Components/SceneComponent.h"

ARacAIWaypoint::ARacAIWaypoint()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}