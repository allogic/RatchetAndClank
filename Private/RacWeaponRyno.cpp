#include "RacWeaponRyno.h"
#include "RacTPSCharacterBase.h"
#include "RacBulletRyno.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ARacWeaponRyno::ARacWeaponRyno()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshWeapon(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Weapons/Ryno/Ryno_SM.Ryno_SM'"));

  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueShot(TEXT("SoundCue'/Game/RatchetAndClank/Sounds/Weapons/Ryno/Ryno_SC.Ryno_SC'"));

  mComponentWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
  mComponentWeapon->SetupAttachment(RootComponent);
  mComponentWeapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  if (staticMeshWeapon.Succeeded()) mComponentWeapon->SetStaticMesh(staticMeshWeapon.Object);

  mBulletSpawnLocations[0] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn0"));
  mBulletSpawnLocations[1] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn1"));
  mBulletSpawnLocations[2] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn2"));
  mBulletSpawnLocations[3] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn3"));
  mBulletSpawnLocations[4] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn4"));
  mBulletSpawnLocations[5] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn5"));
  mBulletSpawnLocations[6] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn6"));
  mBulletSpawnLocations[7] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawn7"));

  FVector exhaustLocation = FVector{ 0.0f, 14.5f, 25.0f };
  mBulletSpawnLocations[0]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, 0.0f, 15.0f });
  mBulletSpawnLocations[1]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, -10.0f, 10.0f });
  mBulletSpawnLocations[2]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, -15.0f, 0.0f });
  mBulletSpawnLocations[3]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, -10.0f, -10.0f });
  mBulletSpawnLocations[4]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, 0.0f, -15.0f });
  mBulletSpawnLocations[5]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, 10.0f, -10.0f });
  mBulletSpawnLocations[6]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, 15.0f, 0.0f });
  mBulletSpawnLocations[7]->SetRelativeLocation(exhaustLocation + FVector{ 70.0f, 10.0f, 10.0f });

  mBulletSpawnLocations[0]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[1]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[2]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[3]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[4]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[5]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[6]->SetupAttachment(mComponentWeapon);
  mBulletSpawnLocations[7]->SetupAttachment(mComponentWeapon);

  mFireRate = 0.2f;

  if (soundCueShot.Succeeded()) mSoundShot = soundCueShot.Object;
}

void ARacWeaponRyno::AttachToCharacter(ARacTPSCharacterBase* Character)
{
  AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

  mComponentWeapon->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));

  mComponentWeapon->SetRelativeLocation(FVector{ 0.0f, -10.0f, 0.0f });
  mComponentWeapon->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });
}

void ARacWeaponRyno::Fire()
{
  mBulletSpawnIndex = (mBulletSpawnIndex + 1) % 8;
  mRecoil = -15.0f;

  GetWorld()->SpawnActor<ARacBulletRyno>(mBulletSpawnLocations[mBulletSpawnIndex]->GetComponentLocation(), mBulletSpawnLocations[mBulletSpawnIndex]->GetComponentRotation());

  UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundShot, GetActorLocation());
}

FTransform ARacWeaponRyno::GetLeftHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  FVector handLocation = SkeletalMesh->GetBoneLocation(TEXT("hand_r"), EBoneSpaces::ComponentSpace);
  FVector gripOffset = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 20.0f, 25.0f, 10.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + gripOffset };
}

FTransform ARacWeaponRyno::GetRightHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_r"), EBoneSpaces::ComponentSpace);
  FVector gripOffset = FVector{ 5.0f, 0.0f, 0.0f };
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 20.0f, 0.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, upperArmLocation + handLocation + gripOffset };
}

FTransform ARacWeaponRyno::GetLeftHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  FVector handLocation = SkeletalMesh->GetBoneLocation(TEXT("hand_r"), EBoneSpaces::ComponentSpace);
  FVector gripOffset = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 20.0f, 25.0f, 10.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + gripOffset };
}

FTransform ARacWeaponRyno::GetRightHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  mRecoil = FMath::Lerp(mRecoil, 0.0f, DeltaTime * 20.0f);

  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_r"), EBoneSpaces::ComponentSpace);
  FVector gripOffset = FVector{ 5.0f, 0.0f, 0.0f };
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 20.0f + mRecoil, 0.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, upperArmLocation + handLocation + gripOffset };
}