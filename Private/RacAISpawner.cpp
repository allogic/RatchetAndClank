#include "RacAISpawner.h"
#include "RacAICharacterBase.h"
#include "RacAIControllerBase.h"

#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"

ARacAISpawner::ARacAISpawner()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshPad(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Misc/Pad/Pad_SM.Pad_SM'"));

	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mComponentPad = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Pad"));
	mComponentPad->SetupAttachment(RootComponent);
	if (staticMeshPad.Succeeded()) mComponentPad->SetStaticMesh(staticMeshPad.Object);
}

void ARacAISpawner::BeginPlay()
{
	Super::BeginPlay();

	if (mSpawnEnabled && mSpawnCharacter)
	{
		GetWorldTimerManager().SetTimer(mSpawnTimer, this, &ARacAISpawner::Spawn, mSpawnRate, true, 0.0f);
	}
}

void ARacAISpawner::Spawn()
{
	if (ARacAICharacterBase* character = GetWorld()->SpawnActor<ARacAICharacterBase>(mSpawnCharacter, GetActorLocation() + mSpawnOffset, FRotator::ZeroRotator))
	{
		character->SetTeam(mTeam);
		character->EnablePerception();

		if (ARacAIControllerBase* controller = GetWorld()->SpawnActor<ARacAIControllerBase>())
		{
			controller->Possess(character);
		}
	}
}