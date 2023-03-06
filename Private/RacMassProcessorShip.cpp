#include "RacMassProcessorShip.h"
#include "RacMassFragments.h"

#include "RacMath.h"

#include "MassCommonFragments.h"

URacMassProcessorShip::URacMassProcessorShip()
{
  bAutoRegisterWithProcessingPhases = true;

  ExecutionFlags = (int)EProcessorExecutionFlags::All;
}

void URacMassProcessorShip::ConfigureQueries()
{
  mEntityQuery.AddRequirement<FTransformFragment>(EMassFragmentAccess::ReadWrite);
  mEntityQuery.AddRequirement<FRacMassFragmentFollower>(EMassFragmentAccess::ReadWrite);
}

void URacMassProcessorShip::Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context)
{
  mEntityQuery.ForEachEntityChunk(EntityManager, Context, [this](FMassExecutionContext& Context) {
      TArrayView<FTransformFragment> transformView = Context.GetMutableFragmentView<FTransformFragment>();
      TArrayView<FRacMassFragmentFollower> followerView = Context.GetMutableFragmentView<FRacMassFragmentFollower>();
      
      float deltaTime = Context.GetDeltaTimeSeconds();
      
      for (int i = 0; i < Context.GetNumEntities(); i++)
      {
        FTransform& transform = transformView[i].GetMutableTransform();
        FRacMassFragmentFollower& follower = followerView[i];

        if (follower.IsFollowing)
        {
          FVector location = transform.GetLocation();
          
          location += follower.Direction * deltaTime * follower.Speed;

          if (FVector::Distance(location, follower.Target) < FVector::Distance(FVector::ZeroVector, follower.Direction * follower.Speed))
          {
            follower.IsFollowing = false;
          }
          else
          {
            transform.SetLocation(location);
          }
        }
        else
        {
          FPaths& randomPath = mPaths[FMath::RandRange(0, mPaths.Num() - 1)];

          FVector offset = FRacMath::RandRange(-1000.0f, 1000.0f);

          FVector end = randomPath.End + offset;
          FVector start = randomPath.Start + offset;

          FVector direction = (end - start);
          direction.Normalize();

          follower.IsFollowing = true;
          follower.Speed = FMath::FRandRange(500.0f, 2000.0f);
          follower.Direction = direction;
          follower.Target = end;

          transform.SetLocation(start);
        }
      }
    });
}