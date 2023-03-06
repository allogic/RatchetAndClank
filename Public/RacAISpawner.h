#pragma once

#include "RacTeam.h"

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RacAISpawner.generated.h"

class ARacAICharacterBase;

class UStaticMeshComponent;

UCLASS()
class RATCHETANDCLANK_API ARacAISpawner : public AActor
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Sub Components
	////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentPad = nullptr;

	////////////////////////////////////////////////////////////////////////
	// Spawn Settings
	////////////////////////////////////////////////////////////////////////

public:

	UPROPERTY(EditAnywhere, Category = RacSpawn)
		bool mSpawnEnabled = true;

	UPROPERTY(EditAnywhere, Category = RacSpawn)
		float mSpawnRate = 5.0f;

	UPROPERTY(EditAnywhere, Category = RacSpawn)
		FVector mSpawnOffset = FVector{ 0.0f, 0.0f, 88.0f };

	UPROPERTY(EditAnywhere, Category = RacSpawn)
		TSubclassOf<ARacAICharacterBase> mSpawnCharacter = nullptr;

	UPROPERTY(EditAnywhere, Category = RacSpawn)
		ERacTeam mTeam = ERacTeam::None;

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacAISpawner();

	////////////////////////////////////////////////////////////////////////
	// Actor Overrides
	////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;

	////////////////////////////////////////////////////////////////////////
	// Spawn System
	////////////////////////////////////////////////////////////////////////

private:

	FTimerHandle mSpawnTimer = FTimerHandle{};

	void Spawn();
};