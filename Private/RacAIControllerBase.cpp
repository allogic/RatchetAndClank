#include "RacAIControllerBase.h"
#include "RacAICharacterBase.h"
#include "RacMath.h"
#include "RacAIWaypoint.h"

#include "NavigationSystem.h"

#include "DrawDebugHelpers.h"

#include "EngineUtils.h"

ARacAIControllerBase::ARacAIControllerBase()
{
  PrimaryActorTick.bCanEverTick = true;
}

void ARacAIControllerBase::BeginPlay()
{
  Super::BeginPlay();
}

void ARacAIControllerBase::Tick(float DeltaTime)
{
  Super::Tick(DeltaTime);

  if (mRotateTowardsGoal)
  {
    RotateTowardsGoal();
  }

  switch (mBehaviorState)
  {
    case EBehaviorState::Idle: mBehaviorState = DoStateIdle(); break;
    case EBehaviorState::FollowPath: mBehaviorState = DoStateFollowPath(); break;
    case EBehaviorState::Patrol: mBehaviorState = DoStatePatrol(); break;
    case EBehaviorState::EncounterEnemy: mBehaviorState = DoStateEncounterEnemy(); break;
    case EBehaviorState::TakeCover: mBehaviorState = DoStateTakeCover(); break;
    case EBehaviorState::Combat: mBehaviorState = DoStateCombat(); break;
  }

  DebugState();
  DebugPath();
  DebugSegment();
}

void ARacAIControllerBase::OnPossess(APawn* InPawn)
{
  Super::OnPossess(InPawn);

  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    for (TActorIterator<ARacAIWaypoint> actorIt{ GetWorld() }; actorIt; ++actorIt)
    {
      mWaypoints.Add(*actorIt);
    }
  }
}

void ARacAIControllerBase::OnUnPossess()
{
  Super::OnUnPossess();
}

void ARacAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
  Super::OnMoveCompleted(RequestID, Result);

  switch (Result.Code)
  {
    case EPathFollowingResult::Blocked:
    case EPathFollowingResult::Aborted:
    case EPathFollowingResult::Invalid:
    case EPathFollowingResult::OffPath:
    case EPathFollowingResult::Success:
    {
      mIsFollowingPath = false;
      mCurrentWaypoint = nullptr;
      break;
    }
  }
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStateIdle()
{
  return EBehaviorState::Patrol;
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStateFollowPath()
{
  if (mIsFollowingPath)
  {
    if (HasFoundEnemies())
    {
      GetPathFollowingComponent()->AbortMove(*this, FPathFollowingResultFlags::MovementStop);

      return EBehaviorState::EncounterEnemy;
    }

    return EBehaviorState::FollowPath;
  }

  return EBehaviorState::Idle;
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStatePatrol()
{
  if (mCurrentWaypoint)
  {
    if (MoveNearLocation(mCurrentWaypoint->GetActorLocation(), mAcceptanceRadius))
    {
      return EBehaviorState::FollowPath;
    }
  }
  else
  {
    mCurrentWaypoint = GetRandomWaypoint();

    return EBehaviorState::Patrol;
  }

  return EBehaviorState::Idle;
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStateEncounterEnemy()
{
  if (mCurrentEnemy)
  {
    if (mCurrentEnemy->IsDead())
    {
      mCurrentEnemy->DecrementReferenceCount();

      DestroyEnemyIfPossible();

      mCurrentEnemy = nullptr;
    }
  }
  else
  {
    mCurrentEnemy = GetRandomEnemy();

    if (mCurrentEnemy)
    {
      mCurrentEnemy->IncrementReferenceCount();

      return EBehaviorState::TakeCover;
    }
  }

  return EBehaviorState::Idle;
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStateTakeCover()
{
  static bool hasTakenCover = false;

  if (hasTakenCover)
  {
    hasTakenCover = false;

    return EBehaviorState::Combat;
  }
  else
  {
    hasTakenCover = true;

    if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
    {
      FVector coverLocation = character->GetActorLocation() + FRacMath::RandomInsideUnitCircle() * 2000.0f;

      if (MoveNearLocation(coverLocation, mAcceptanceRadius))
      {
        return EBehaviorState::FollowPath;
      }
    }
  }

  return EBehaviorState::Idle;
}

ARacAIControllerBase::EBehaviorState ARacAIControllerBase::DoStateCombat()
{
  if (mCurrentEnemy && mCurrentEnemy->IsAlive())
  {
    mCurrentEnemy->DecreaseHealth(FMath::RandRange(0, 10));

    return EBehaviorState::Combat;
  }

  return EBehaviorState::Idle;
}

bool ARacAIControllerBase::MoveNearLocation(const FVector& Location, float AcceptanceRadius)
{
  if (UNavigationSystemV1* navigationSystem = UNavigationSystemV1::GetCurrent(GetWorld()))
  {
    FNavLocation navLocation;

    if (navigationSystem->GetRandomPointInNavigableRadius(Location, AcceptanceRadius, navLocation, nullptr))
    {
      EPathFollowingRequestResult::Type result = MoveToLocation(navLocation.Location);

      if (result == EPathFollowingRequestResult::RequestSuccessful)
      {
        mIsFollowingPath = true;

        return true;
      }
    }
  }

  return false;
}

FVector ARacAIControllerBase::Seperation(float Strength)
{
  FVector steering = FVector::ZeroVector;

  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    for (ARacAICharacterBase* allyCharacter : character->GetPerceivedAllies())
    {
      FVector selfLocation = character->GetActorLocation();
      FVector allyLocation = allyCharacter->GetActorLocation();
  
      float distance = FVector::Distance(selfLocation, allyLocation);
      FVector direction = (selfLocation - allyLocation) / distance;
  
      steering += direction;
    }
  
    for (ARacAICharacterBase* enemyCharacter : character->GetPerceivedEnemies())
    {
      FVector selfLocation = character->GetActorLocation();
      FVector enemyLocation = enemyCharacter->GetActorLocation();

      float distance = FVector::Distance(selfLocation, enemyLocation);
      FVector direction = (selfLocation - enemyLocation) / distance;

      steering += direction;
    }

    int count = character->GetPerceivedAllies().Num() + character->GetPerceivedEnemies().Num();
  
    if (count)
    {
      steering /= count;
      steering *= Strength;
    }
  }

  return steering;
}

FVector ARacAIControllerBase::Alignment(float Strength)
{
  FVector steering = FVector::ZeroVector;

  return steering;
}

FVector ARacAIControllerBase::Cohesion(float Strength)
{
  FVector steering = FVector::ZeroVector;

  return steering;
}

ARacAIWaypoint* ARacAIControllerBase::GetRandomWaypoint()
{
  if (mWaypoints.Num() > 0)
  {
    return mWaypoints[FMath::RandRange(0, mWaypoints.Num() - 1)];
  }

  return nullptr;
}

bool ARacAIControllerBase::HasFoundEnemies()
{
  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    bool found = false;

    for (ARacAICharacterBase* enemyCharacter : character->GetPerceivedEnemies())
    {
      if (enemyCharacter->IsAlive())
      {
        found = true;
        break;
      }
    }

    return found;
  }

  return false;
}

bool ARacAIControllerBase::CanSeeEnemy()
{
  return false;
}

ARacAICharacterBase* ARacAIControllerBase::GetRandomEnemy()
{
  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    for (ARacAICharacterBase* enemyCharacter : character->GetPerceivedEnemies())
    {
      if (enemyCharacter->IsAlive())
      {
        return enemyCharacter;
      }
    }
  }

  return nullptr;
}

void ARacAIControllerBase::RotateTowardsGoal()
{
  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    FVector direction = FVector::ZeroVector;

    if (mCurrentEnemy)
    {
      direction = mCurrentEnemy->GetActorLocation() - character->GetActorLocation();
    }
    else if (mCurrentWaypoint)
    {
      direction = mCurrentWaypoint->GetActorLocation() - character->GetActorLocation();
    }

    character->SetActorRotation(FRotationMatrix::MakeFromX(direction).Rotator());
  }
}

void ARacAIControllerBase::DestroyEnemyIfPossible()
{
  if (mCurrentEnemy && !mCurrentEnemy->HasControllerReferences())
  {
    if (ARacAIControllerBase* enemyController = Cast<ARacAIControllerBase>(mCurrentEnemy->GetController()))
    {
      enemyController->UnPossess();

      mCurrentEnemy->Destroy();
      enemyController->Destroy();
    }
  }
}

void ARacAIControllerBase::DebugState()
{
  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    if (mCurrentWaypoint)
    {
      DrawDebugLine(GetWorld(), character->GetActorLocation(), mCurrentWaypoint->GetActorLocation(), FColor::Blue, false, -1.0f, 0, 3.0f);
    }

    if (mCurrentEnemy)
    {
      DrawDebugLine(GetWorld(), character->GetActorLocation(), mCurrentEnemy->GetActorLocation(), FColor::Orange, false, -1.0f, 0, 3.0f);
    }

    for (ARacAICharacterBase* allyCharacter : character->GetPerceivedAllies())
    {
      DrawDebugLine(GetWorld(), character->GetActorLocation(), allyCharacter->GetActorLocation(), FColor::Green, false, -1.0f, 0, 3.0f);
    }

    for (ARacAICharacterBase* enemyCharacter : character->GetPerceivedEnemies())
    {
      DrawDebugLine(GetWorld(), character->GetActorLocation(), enemyCharacter->GetActorLocation(), FColor::Red, false, -1.0f, 0, 3.0f);
    }
  }
}

void ARacAIControllerBase::DebugPath()
{
  if (UPathFollowingComponent* pathFollowingComponent = GetPathFollowingComponent())
  {
    if (pathFollowingComponent->HasValidPath())
    {
      FNavPathSharedPtr path = pathFollowingComponent->GetPath();
      int pathSize = path->GetPathPoints().Num();
      FVector floorOffset = FVector{ 0.0f, 0.0f, 88.0f };

      for (int32 i = 0; i < (pathSize - 1); ++i)
      {
        FVector currLocation = path->GetPathPointLocation(i).Position;
        FVector nextLocation = path->GetPathPointLocation(i + 1).Position;

        DrawDebugLine(GetWorld(), currLocation, currLocation + floorOffset, FColor::Yellow, false, -1.0f, 0, 3.0f);

        if (i < (pathSize - 1))
        {
          DrawDebugLine(GetWorld(), currLocation, nextLocation, FColor::White, false, -1.0f, 0, 3.0f);
        }
      }
    }
  }
}

void ARacAIControllerBase::DebugSegment()
{
  if (ARacAICharacterBase* character = Cast<ARacAICharacterBase>(GetCharacter()))
  {
    if (UPathFollowingComponent* pathFollowingComponent = GetPathFollowingComponent())
    {
      if (pathFollowingComponent->HasValidPath())
      {
        FNavPathSharedPtr path = pathFollowingComponent->GetPath();
        int pathIndex = pathFollowingComponent->GetNextPathIndex();

        DrawDebugLine(GetWorld(), character->GetActorLocation(), path->GetPathPointLocation(pathIndex).Position, FColor::Yellow, false, -1.0f, 0, 3.0f);
      }
    }
  }
}