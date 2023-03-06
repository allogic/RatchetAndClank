#pragma once

#include "RacTeam.h"

#include "CoreMinimal.h"

#include "GameFramework/Character.h"

#include "RacAICharacterBase.generated.h"

class USphereComponent;
class UPhysicalAnimationComponent;

UCLASS()
class RATCHETANDCLANK_API ARacAICharacterBase : public ACharacter
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Sub Components
	////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = RacComponent)
		USphereComponent* mComponentPerception = nullptr;

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UPhysicalAnimationComponent* mComponentPhysicalAnimation = nullptr;

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacAICharacterBase();

	////////////////////////////////////////////////////////////////////////
	// Character Overrides
	////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	////////////////////////////////////////////////////////////////////////
	// Perception System
	////////////////////////////////////////////////////////////////////////

public:

	void EnablePerception();

	inline const TArray<ARacAICharacterBase*>& GetPerceivedAllies() const { return mPerceivedAllies; }
	inline const TArray<ARacAICharacterBase*>& GetPerceivedEnemies() const { return mPerceivedEnemies; }

private:

	UPROPERTY(EditAnywhere, Category = RacPerception)
		float mPerceptionRadius = 2000.0f;

	TArray<ARacAICharacterBase*> mPerceivedAllies = {};
	TArray<ARacAICharacterBase*> mPerceivedEnemies = {};

	UFUNCTION()
		void OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	////////////////////////////////////////////////////////////////////////
	// Team System
	////////////////////////////////////////////////////////////////////////

public:

	inline ERacTeam GetTeam() const { return mTeam; }

	inline void SetTeam(ERacTeam Value) { mTeam = Value; }

private:

	UPROPERTY(EditAnywhere, Category = RacTeam)
		ERacTeam mTeam = ERacTeam::None;

	////////////////////////////////////////////////////////////////////////
	// Combat System
	////////////////////////////////////////////////////////////////////////

public:

	inline void IncrementReferenceCount() { mNumControllerReferences++; }
	inline void DecrementReferenceCount() { mNumControllerReferences--; }

	inline bool HasControllerReferences() const { return mNumControllerReferences > 0; }

private:

	int mNumControllerReferences = 0;

	////////////////////////////////////////////////////////////////////////
	// Health System
	////////////////////////////////////////////////////////////////////////

public:

	inline bool IsAlive() const { return mHealth > 0; }
	inline bool IsDead() const { return mHealth <= 0; }
	inline int GetHealth() const { return mHealth; }

	inline void DecreaseHealth(int Value) { mHealth -= Value; }

private:

	int mHealth = 10000;

	////////////////////////////////////////////////////////////////////////
	// IK System
	////////////////////////////////////////////////////////////////////////

private:

	void UpdateHandIK(float DeltaTime);
};