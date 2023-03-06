#include "RacWeaponViper.h"
#include "RacTPSCharacterBase.h"
#include "RacBulletViper.h"

#include "Kismet/GameplayStatics.h"

#include "Sound/SoundCue.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

ARacWeaponViper::ARacWeaponViper()
{
  static ConstructorHelpers::FObjectFinder<UStaticMesh> staticMeshWeapon(TEXT("StaticMesh'/Game/RatchetAndClank/Meshes/Weapons/Viper/Viper_SM.Viper_SM'"));

  static ConstructorHelpers::FObjectFinder<USoundCue> soundCueShot(TEXT("SoundCue'/Game/RatchetAndClank/Sounds/Weapons/Viper/Viper_SC.Viper_SC'"));

  static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraSystemShell(TEXT("NiagaraSystem'/Game/RatchetAndClank/Effects/ViperShell_NS.ViperShell_NS'"));

  mComponentWeaponLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponLeft"));
  mComponentWeaponLeft->SetupAttachment(RootComponent);
  mComponentWeaponLeft->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  if (staticMeshWeapon.Succeeded()) mComponentWeaponLeft->SetStaticMesh(staticMeshWeapon.Object);

  mComponentWeaponRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponRight"));
  mComponentWeaponRight->SetupAttachment(RootComponent);
  mComponentWeaponRight->SetCollisionEnabled(ECollisionEnabled::NoCollision);
  if (staticMeshWeapon.Succeeded()) mComponentWeaponRight->SetStaticMesh(staticMeshWeapon.Object);

  mBulletSpawnLocations[0] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnLeft"));
  mBulletSpawnLocations[0]->SetupAttachment(mComponentWeaponLeft);
  mBulletSpawnLocations[0]->SetRelativeLocation(FVector{ 42.0f, 0.0f, 10.0f });

  mBulletSpawnLocations[1] = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnRight"));
  mBulletSpawnLocations[1]->SetupAttachment(mComponentWeaponRight);
  mBulletSpawnLocations[1]->SetRelativeLocation(FVector{ 42.0f, 0.0f, 10.0f });

  mShellSpawnLocations[0] = CreateDefaultSubobject<USceneComponent>(TEXT("ShellSpawnLeft"));
  mShellSpawnLocations[0]->SetupAttachment(mComponentWeaponLeft);
  mShellSpawnLocations[0]->SetRelativeLocation(FVector{ 22.0f, 0.0f, 20.0f });

  mShellSpawnLocations[1] = CreateDefaultSubobject<USceneComponent>(TEXT("ShellSpawnRight"));
  mShellSpawnLocations[1]->SetupAttachment(mComponentWeaponRight);
  mShellSpawnLocations[1]->SetRelativeLocation(FVector{ 22.0f, 0.0f, 20.0f });

  mFireRate = 0.14f;

  if (soundCueShot.Succeeded()) mSoundShot = soundCueShot.Object;

  if (niagaraSystemShell.Succeeded()) mNiagaraShell = niagaraSystemShell.Object;
}

void ARacWeaponViper::AttachToCharacter(ARacTPSCharacterBase* Character)
{
  AttachToActor(Character, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

  mComponentWeaponLeft->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_l"));
  mComponentWeaponRight->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r"));

  mComponentWeaponLeft->SetRelativeLocation(FVector{ 0.0f, -10.0f, 0.0f });
  mComponentWeaponRight->SetRelativeLocation(FVector{ 0.0f, -10.0f, 0.0f });

  mComponentWeaponLeft->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });
  mComponentWeaponRight->SetRelativeRotation(FRotator{ 0.0f, -90.0f, 0.0f });
}

void ARacWeaponViper::Fire()
{
  if (mActiveWeaponIndex == 0) mRecoilLeft = -7.0f;
  if (mActiveWeaponIndex == 1) mRecoilRight = -7.0f;

  mActiveWeaponIndex = (mActiveWeaponIndex + 1) % 2;

  GetWorld()->SpawnActor<ARacBulletViper>(mBulletSpawnLocations[mActiveWeaponIndex]->GetComponentLocation(), mBulletSpawnLocations[mActiveWeaponIndex]->GetComponentRotation());

  UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundShot, GetActorLocation());

  UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagaraShell, mShellSpawnLocations[mActiveWeaponIndex]->GetComponentLocation());
}

FTransform ARacWeaponViper::GetLeftHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_l"), EBoneSpaces::ComponentSpace);
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 20.0f, -30.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + upperArmLocation };
}

FTransform ARacWeaponViper::GetRightHandRestTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_r"), EBoneSpaces::ComponentSpace);
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 20.0f, -30.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + upperArmLocation };
}

FTransform ARacWeaponViper::GetLeftHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  mRecoilLeft = FMath::Lerp(mRecoilLeft, 0.0f, DeltaTime * 20.0f);

  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_l"), EBoneSpaces::ComponentSpace);
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 40.0f + mRecoilLeft, 0.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + upperArmLocation };
}

FTransform ARacWeaponViper::GetRightHandAimTransform(USkeletalMeshComponent* SkeletalMesh, float CameraPitch, float DeltaTime)
{
  mRecoilRight = FMath::Lerp(mRecoilRight, 0.0f, DeltaTime * 20.0f);

  FVector upperArmLocation = SkeletalMesh->GetBoneLocation(TEXT("upperarm_r"), EBoneSpaces::ComponentSpace);
  FVector handLocation = FRotator{ 0.0f, 0.0f, -CameraPitch }.RotateVector(FVector{ 0.0f, 40.0f + mRecoilRight, 0.0f });

  return FTransform{ FRotator{ 0.0f, 180.0f, CameraPitch }, handLocation + upperArmLocation };
}