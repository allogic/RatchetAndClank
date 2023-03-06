#pragma once

#include "CoreMinimal.h"

struct RATCHETANDCLANK_API FRacMath
{
  ////////////////////////////////////////////////////////////////////////
  // Random Utils
  ////////////////////////////////////////////////////////////////////////

  static FVector RandRange(float Min, float Max);
  static FVector RandRange(const FVector& Min, const FVector& Max);

  ////////////////////////////////////////////////////////////////////////
  // Remapping Utils
  ////////////////////////////////////////////////////////////////////////

	static float Remap(float Value, float Low0, float High0, float Low1, float High1);
  static FVector Remap(const FVector& Value, float Low0, float High0, float Low1, float High1);

  ////////////////////////////////////////////////////////////////////////
  // Spline Utils
  ////////////////////////////////////////////////////////////////////////

  struct FCatmullRomSegment
  {
    FVector A, B, C, D;
  };

  static FCatmullRomSegment ComputeCatmullRom(const FVector& P0, const FVector& P1, const FVector& P2, const FVector& P3, float Alpha = 0.5f, float Tension = 0.0f);
  static FVector SampleCatmullRom(const FCatmullRomSegment& Segment, float Time);

  ////////////////////////////////////////////////////////////////////////
  // Geometric Utils
  ////////////////////////////////////////////////////////////////////////

  static FVector RandomInsideUnitCircle();
  static FVector RandomInsideUnitSphere();
  static FVector RandomInsideRect(const FVector& P, const FVector& S);

  static void UniformPointsOnSphere(int N, TArray<FVector>& Points);
};