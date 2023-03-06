#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "RacMath.h"

#include "RacDropShip.generated.h"

class UStaticMeshComponent;

UCLASS()
class RATCHETANDCLANK_API ARacDropShip : public AActor
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Sub Components
	////////////////////////////////////////////////////////////////////////

protected:

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentShip = nullptr;

	UPROPERTY(EditAnywhere, Category = RacComponent)
		UStaticMeshComponent* mComponentDoor = nullptr;

	////////////////////////////////////////////////////////////////////////
	// Constructor
	////////////////////////////////////////////////////////////////////////

public:

	ARacDropShip();

	////////////////////////////////////////////////////////////////////////
	// Actor Overrides
	////////////////////////////////////////////////////////////////////////

protected:

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	////////////////////////////////////////////////////////////////////////
	// Path Prediction
	////////////////////////////////////////////////////////////////////////

private:

	FVector mTargetLocation = {};

	float mTargetSeekTension = 0.25f;

	bool mComputePath = true;
	bool mPathInit = true;

	int mNumSphereSamples = 16;

	struct FPathNode
	{
		FVector Location;
		FQuat Rotation;
	};

	TArray<FPathNode> mPath = {};
	TArray<FVector> mSphericalDirections = {};

	void PredictPath();

	float ComputePath(FVector Location, FQuat Rotation, int Samples, bool Init);
	float ComputeSubPath(FVector Location, FQuat Rotation, int Samples);

	bool BoxIntersectionAtLocation(FHitResult& hitResult, const FVector& Location, const FVector& Size, const FQuat& Rotation);
	bool SphereIntersectionAtLocation(FHitResult& hitResult, const FVector& Location, float Radius);

	void RenderPathNodes();
	void RenderPathSpline();

	////////////////////////////////////////////////////////////////////////
	// Path Following
	////////////////////////////////////////////////////////////////////////

private:

	FRacMath::FCatmullRomSegment mSegment = {};

	float mSegmentStep = 0.0f;
	float mSegmentAccum = 0.0f;

	FVector mPrevSegmentSampleLocation = {};

	void FollowPath();

	float ComputeSegment(float SampleRate);
};