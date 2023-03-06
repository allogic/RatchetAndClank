#include "RacDropShip.h"
#include "RacMath.h"

#include "Engine/StaticMesh.h"

#include "Components/StaticMeshComponent.h"

ARacDropShip::ARacDropShip()
{
	static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshShip(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Ships/DropShip/Ship_SM.Ship_SM'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshDoor(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Ships/DropShip/Door_SM.Door_SM'"));

	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	mComponentShip = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ship"));
	mComponentShip->SetupAttachment(RootComponent);
	mComponentShip->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (staticMeshShip.Succeeded()) mComponentShip->SetStaticMesh(staticMeshShip.Object);

	mComponentDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door"));
	mComponentDoor->SetupAttachment(RootComponent);
	mComponentDoor->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	if (staticMeshDoor.Succeeded()) mComponentDoor->SetStaticMesh(staticMeshDoor.Object);
}

void ARacDropShip::BeginPlay()
{
	Super::BeginPlay();

	FRacMath::UniformPointsOnSphere(mNumSphereSamples, mSphericalDirections);
}

void ARacDropShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	PredictPath();
	FollowPath();

	RenderPathNodes();
	RenderPathSpline();
}

void ARacDropShip::PredictPath()
{
	if (mComputePath)
	{
		ComputePath(GetActorLocation(), GetActorQuat(), 4, mPathInit);

		mSegmentStep = ComputeSegment(0.01f);

		mComputePath = false;
		mPathInit = false;
	}
}

float ARacDropShip::ComputePath(FVector Location, FQuat Rotation, int Samples, bool Init)
{
	float pathLength = 0.0f;

	if (Init)
	{
		mPath.Emplace(FPathNode{ Location - Rotation.GetForwardVector() * 10000.0f, Rotation });
		mPath.Emplace(FPathNode{ Location, Rotation });
	}

	for (int i = 0; i < Samples; i++)
	{
		FVector targetDir = (mTargetLocation - Location);
		targetDir.Normalize();

		FHitResult hitResult = {};
		FQuat nextRotation = FQuat::FastLerp(Rotation, FRotationMatrix::MakeFromX(targetDir).Rotator().Quaternion(), mTargetSeekTension);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, Location + nextRotation.GetForwardVector() * 10000.0f, ECollisionChannel::ECC_Visibility))
		{
			Location = hitResult.Location + hitResult.Normal * 1000.0f;

			FVector subDirection = {};
			float maxSubLength = 0.0f;

			for (int j = 0; j < mSphericalDirections.Num(); j++)
			{
				float subLength = ComputeSubPath(Location, mSphericalDirections[j].ToOrientationQuat(), 5);

				if (subLength > maxSubLength)
				{
					subDirection = mSphericalDirections[j];
					maxSubLength = subLength;
				}
			}

			Rotation = subDirection.ToOrientationQuat();
		}

		Rotation = FQuat::FastLerp(Rotation, FRotationMatrix::MakeFromX(targetDir).Rotator().Quaternion(), mTargetSeekTension);
		Location += Rotation.GetForwardVector() * 10000.0f;

		mPath.Emplace(FPathNode{ Location, Rotation });

		pathLength += 10000.0f;
	}

	mPath.RemoveAt(mPath.Num() - 1);

	return pathLength;
}

float ARacDropShip::ComputeSubPath(FVector Location, FQuat Rotation, int Samples)
{
	float pathLength = 0.0f;

	for (int i = 0; i < Samples; i++)
	{
		FVector targetDir = (mTargetLocation - Location);
		targetDir.Normalize();

		FHitResult hitResult = {};
		FQuat nextRotation = FQuat::FastLerp(Rotation, FRotationMatrix::MakeFromX(targetDir).Rotator().Quaternion(), mTargetSeekTension);

		if (GetWorld()->LineTraceSingleByChannel(hitResult, Location, Location + nextRotation.GetForwardVector() * 10000.0f, ECollisionChannel::ECC_Visibility))
		{
			break;
		}
		else
		{
			Rotation = FQuat::FastLerp(Rotation, FRotationMatrix::MakeFromX(targetDir).Rotator().Quaternion(), mTargetSeekTension);
			Location += Rotation.GetForwardVector() * 10000.0f;

			pathLength += 10000.0f;
		}
	}

	return pathLength;
}

bool ARacDropShip::BoxIntersectionAtLocation(FHitResult& hitResult, const FVector& Location, const FVector& Size, const FQuat& Rotation)
{
	FCollisionShape collisionShape = {};
	collisionShape.SetBox((FVector3f)Size);

	return GetWorld()->SweepSingleByChannel(hitResult, Location, Location, Rotation, ECollisionChannel::ECC_Visibility, collisionShape);
}

bool ARacDropShip::SphereIntersectionAtLocation(FHitResult& hitResult, const FVector& Location, float Radius)
{
	FCollisionShape collisionShape = {};
	collisionShape.SetSphere(Radius);

	return GetWorld()->SweepSingleByChannel(hitResult, Location, Location, FQuat::Identity, ECollisionChannel::ECC_Visibility, collisionShape);
}

void ARacDropShip::RenderPathNodes()
{
	FVector prevLocation = mPath[1].Location;

	for (int j = 2; j < (mPath.Num() - 1); j++)
	{
		DrawDebugPoint(GetWorld(), prevLocation, 10.0f, FColor::Yellow, false, -1.0f, 0);
		DrawDebugLine(GetWorld(), prevLocation, mPath[j].Location, FColor::Yellow, false, -1.0f, 0, 50.0f);

		prevLocation = mPath[j].Location;
	}

	FVector lastLocation = mPath[mPath.Num() - 2].Location;
	DrawDebugPoint(GetWorld(), lastLocation, 10.0f, FColor::Yellow, false, -1.0f, 0);
}

void ARacDropShip::RenderPathSpline()
{
	FVector prevLocation = mPath[1].Location;

	float step = 1.0f / 10.0f;

	for (float t = 0.0f; t < 1.0f; t += step)
	{
		FVector sampleLocation = FRacMath::SampleCatmullRom(mSegment, t);

		FVector targetDir = (prevLocation - sampleLocation);
		targetDir.Normalize();
		FQuat sampleRotation = FRotationMatrix::MakeFromX(targetDir).ToQuat();

		if (t > 0.0f)
		{
			DrawDebugBox(GetWorld(), sampleLocation, FVector{ 1000.0f, 500.0f, 300.0f }, sampleRotation, FColor::Green, false, -1.0f, 0, 10.0f);
		}

		DrawDebugLine(GetWorld(), prevLocation, sampleLocation, FColor::Red, false, -1.0f, 0, 50.0f);

		prevLocation = sampleLocation;
	}
}

void ARacDropShip::FollowPath()
{
	if (mPath.Num() > 3)
	{
		FVector sampleLocation = FRacMath::SampleCatmullRom(mSegment, mSegmentAccum);

		FVector targetDir = (mPrevSegmentSampleLocation - sampleLocation);
		targetDir.Normalize();

		FQuat sampleRotation = FRotationMatrix::MakeFromX(targetDir).ToQuat();

		mPrevSegmentSampleLocation = sampleLocation;

		SetActorLocation(sampleLocation);
		SetActorRotation(sampleRotation);

		mSegmentAccum += mSegmentStep;

		if (mSegmentAccum >= (1.0f - mSegmentStep))
		{
			mSegmentAccum = 0.0f;

			mPath.RemoveAt(0);

			ComputePath(mPath[mPath.Num() - 1].Location, mPath[mPath.Num() - 1].Rotation, 2, false);

			mSegmentStep = ComputeSegment(0.01f);
		}
	}
}

float ARacDropShip::ComputeSegment(float SampleRate)
{
	float segmentLength = 0.0f;

	if (mPath.Num() > 3)
	{
		mSegment = FRacMath::ComputeCatmullRom(mPath[0].Location, mPath[1].Location, mPath[2].Location, mPath[3].Location, 0.5f, -0.25f);
	}

	FVector prevLocation = mPath[1].Location;

	for (float t = 0; t < 1.0f; t += SampleRate)
	{
		FVector sampleLocation = FRacMath::SampleCatmullRom(mSegment, t);
		segmentLength = FVector::Distance(prevLocation, sampleLocation);
		prevLocation = sampleLocation;
	}

	return 1.0f / segmentLength;
}