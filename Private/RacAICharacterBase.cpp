#include "RacAICharacterBase.h"
#include "RacAIControllerBase.h"
#include "RacCollisionChannels.h"
#include "RacAIAnimInstance.h"

#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicalAnimationComponent.h"

ARacAICharacterBase::ARacAICharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ERacCollisionChannel::ECC_Bullet, ECollisionResponse::ECR_Ignore);

	mComponentPerception = CreateDefaultSubobject<USphereComponent>(TEXT("Perception"));
	mComponentPerception->SetupAttachment(RootComponent);
	mComponentPerception->SetSphereRadius(mPerceptionRadius);
	mComponentPerception->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mComponentPerception->SetCollisionObjectType(ERacCollisionChannel::ECC_AI);
	mComponentPerception->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	mComponentPerception->SetCollisionResponseToChannel(ERacCollisionChannel::ECC_AI, ECollisionResponse::ECR_Overlap);
	mComponentPerception->OnComponentBeginOverlap.AddDynamic(this, &ARacAICharacterBase::OnBeginOverlap);
	mComponentPerception->OnComponentEndOverlap.AddDynamic(this, &ARacAICharacterBase::OnEndOverlap);

	mComponentPhysicalAnimation = CreateDefaultSubobject<UPhysicalAnimationComponent>(TEXT("PhysicalAnimation"));
	mComponentPhysicalAnimation->SetStrengthMultiplyer(1.0f);
}

void ARacAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	FPhysicalAnimationData physicalAnimationData{};
	physicalAnimationData.bIsLocalSimulation = false;
	physicalAnimationData.OrientationStrength = 1000.0f;
	physicalAnimationData.AngularVelocityStrength = 100.0f;
	physicalAnimationData.PositionStrength = 1000.0f;
	physicalAnimationData.VelocityStrength = 100.0f;

	mComponentPhysicalAnimation->SetSkeletalMeshComponent(GetMesh());
	mComponentPhysicalAnimation->ApplyPhysicalAnimationSettingsBelow(TEXT("spine_01"), physicalAnimationData);

	GetMesh()->SetAllBodiesBelowSimulatePhysics(TEXT("spine_01"), true);
}

void ARacAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateHandIK(DeltaTime);
}

void ARacAICharacterBase::EnablePerception()
{
	mComponentPerception->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
}

void ARacAICharacterBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this)
	{
		if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(OtherActor))
		{
			if (character->GetTeam() == mTeam)
			{
				mPerceivedAllies.Add(character);
			}
			else
			{
				mPerceivedEnemies.Add(character);
			}
		}
	}
}

void ARacAICharacterBase::OnEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != this)
	{
		if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(OtherActor))
		{
			if (character->GetTeam() == mTeam)
			{
				mPerceivedAllies.Remove(character);
			}
			else
			{
				mPerceivedEnemies.Remove(character);
			}
		}
	}
}

void ARacAICharacterBase::UpdateHandIK(float DeltaTime)
{
	if (USkeletalMeshComponent* skeletalMesh = GetMesh())
	{
		if (URacAIAnimInstance* animInstance = Cast<URacAIAnimInstance>(skeletalMesh->GetAnimInstance()))
		{
			float handLeftBlend = 0.0f;
			float handRightBlend = 0.0f;

			FTransform handLeftTransform = FTransform::Identity;
			FTransform handRightTransform = FTransform::Identity;

			if (ARacAIControllerBase* controller = Cast<ARacAIControllerBase>(GetController()))
			{
				if (ARacAICharacterBase* enemyCharacter = controller->GetCurrentEnemy())
				{
					handRightBlend = 0.0f;
					handLeftBlend = 1.0f;

					FVector upperArmLeftLocation = skeletalMesh->GetBoneLocation(TEXT("upperarm_l"), EBoneSpaces::ComponentSpace);
					FVector upperArmRightLocation = skeletalMesh->GetBoneLocation(TEXT("upperarm_r"), EBoneSpaces::ComponentSpace);

					handLeftTransform = FTransform{ FRotator{ 0.0f, 0.0f, 0.0f }, upperArmLeftLocation + FVector::RightVector * 1000.0f };
					handRightTransform = FTransform{ FRotator{ 0.0f, 0.0f, 0.0f }, upperArmRightLocation + FVector::RightVector * 1000.0f };
				}
			}

			animInstance->mIKHandLeftBlend = FMath::Lerp(animInstance->mIKHandLeftBlend, handLeftBlend, DeltaTime * 10.0f);
			animInstance->mIKHandRightBlend = FMath::Lerp(animInstance->mIKHandRightBlend, handRightBlend, DeltaTime * 10.0f);
			animInstance->mIKHandLeftTransform = handLeftTransform;
			animInstance->mIKHandRightTransform = handRightTransform;
		}
	}
}