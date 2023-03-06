#include "RacCrateBase.h"
#include "RacCollisionChannels.h"

#include "Components/StaticMeshComponent.h"

#include "Kismet/GameplayStatics.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "Sound/SoundCue.h"

ARacCrateBase::ARacCrateBase()
{
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraSystemBoltBurst(TEXT("NiagaraSystem'/Game/RatchetAndClank/Effects/BoltBurst_NS.BoltBurst_NS'"));
	static ConstructorHelpers::FObjectFinder<UNiagaraSystem> niagaraSystemCrateExplosion(TEXT("NiagaraSystem'/Game/RatchetAndClank/Effects/CrateExplosion_NS.CrateExplosion_NS'"));

	PrimaryActorTick.bCanEverTick = true;

	mComponentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	mComponentMesh->SetupAttachment(RootComponent);
	mComponentMesh->SetCollisionObjectType(ERacCollisionChannel::ECC_Crate);
	mComponentMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	mComponentMesh->SetCollisionResponseToChannel(ERacCollisionChannel::ECC_Bullet, ECollisionResponse::ECR_Overlap);
	mComponentMesh->OnComponentBeginOverlap.AddDynamic(this, &ARacCrateBase::OnBeginOverlap);

	if (niagaraSystemBoltBurst.Succeeded()) mNiagaraBoltBurst = niagaraSystemBoltBurst.Object;
	if (niagaraSystemCrateExplosion.Succeeded()) mNiagaraCrateExplosion = niagaraSystemCrateExplosion.Object;
}

void ARacCrateBase::BeginPlay()
{
	Super::BeginPlay();
}

void ARacCrateBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARacCrateBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool FromSweep, const FHitResult& SweepResult)
{
	Destroy();

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagaraBoltBurst, GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), mNiagaraCrateExplosion, GetActorLocation());

	UGameplayStatics::PlaySoundAtLocation(GetWorld(), mSoundExplosion, GetActorLocation());
}