#pragma once

#include "CoreMinimal.h"

#include "AIController.h"

#include "RacAIControllerBase.generated.h"

class ARacAIWaypoint;
class ARacAICharacterBase;

UCLASS()
class RATCHETANDCLANK_API ARacAIControllerBase : public AAIController
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Constructor
  ////////////////////////////////////////////////////////////////////////

public:

  ARacAIControllerBase();

  ////////////////////////////////////////////////////////////////////////
  // Controller Overrides
  ////////////////////////////////////////////////////////////////////////

protected:

  virtual void BeginPlay() override;
  virtual void Tick(float DeltaTime) override;
  virtual void OnPossess(APawn* InPawn) override;
  virtual void OnUnPossess() override;
  virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;

  ////////////////////////////////////////////////////////////////////////
  // AI Brain
  ////////////////////////////////////////////////////////////////////////

public:

  inline ARacAIWaypoint* GetCurrentWaypoint() const { return mCurrentWaypoint; }
  inline ARacAICharacterBase* GetCurrentEnemy() const { return mCurrentEnemy; }

private:

  enum struct EBehaviorState
  {
    Idle,
    FollowPath,
    Patrol,
    EncounterEnemy,
    TakeCover,
    Combat,
  };

  EBehaviorState mBehaviorState = EBehaviorState::Idle;

  EBehaviorState DoStateIdle();
  EBehaviorState DoStateFollowPath();
  EBehaviorState DoStatePatrol();
  EBehaviorState DoStateEncounterEnemy();
  EBehaviorState DoStateTakeCover();
  EBehaviorState DoStateCombat();

  ////////////////////////////////////////////////////////////////////////
  // Path Following
  ////////////////////////////////////////////////////////////////////////

private:

  bool mIsFollowingPath = false;

  float mAcceptanceRadius = 50.0f;

  bool MoveNearLocation(const FVector& Location, float AcceptanceRadius);

  FVector Seperation(float Strength);
  FVector Alignment(float Strength);
  FVector Cohesion(float Strength);

  ////////////////////////////////////////////////////////////////////////
  // Patrolling System
  ////////////////////////////////////////////////////////////////////////

private:

  TArray<ARacAIWaypoint*> mWaypoints = {};

  ARacAIWaypoint* mCurrentWaypoint = nullptr;

  ARacAIWaypoint* GetRandomWaypoint();

  ////////////////////////////////////////////////////////////////////////
  // Perception System
  ////////////////////////////////////////////////////////////////////////

private:

  ARacAICharacterBase* mCurrentEnemy = nullptr;

  bool HasFoundEnemies();
  bool CanSeeEnemy();
  ARacAICharacterBase* GetRandomEnemy();

  ////////////////////////////////////////////////////////////////////////
  // Locomotion System
  ////////////////////////////////////////////////////////////////////////

private:

  bool mRotateTowardsGoal = false;

  void RotateTowardsGoal();

  ////////////////////////////////////////////////////////////////////////
  // Combat System
  ////////////////////////////////////////////////////////////////////////

private:

  void DestroyEnemyIfPossible();

  ////////////////////////////////////////////////////////////////////////
  // Debugging
  ////////////////////////////////////////////////////////////////////////

private:

  void DebugState();
  void DebugPath();
  void DebugSegment();
};