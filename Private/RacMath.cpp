#include "RacMath.h"

#include "MathUtil.h"

FVector FRacMath::RandRange(float Min, float Max)
{
  return FVector{
    FMath::FRandRange(Min, Max),
    FMath::FRandRange(Min, Max),
    FMath::FRandRange(Min, Max),
  };
}

FVector FRacMath::RandRange(const FVector& Min, const FVector& Max)
{
  return FVector{
    FMath::FRandRange(Min.X, Max.X),
    FMath::FRandRange(Min.Y, Max.Y),
    FMath::FRandRange(Min.Z, Max.Z),
  };
}

float FRacMath::Remap(float Value, float Low0, float High0, float Low1, float High1)
{
  return Low1 + (Value - Low0) * (High1 - Low1) / (High0 - Low0);
}

FVector FRacMath::Remap(const FVector& Value, float Low0, float High0, float Low1, float High1)
{
  return FVector{
    Remap(Value.X, Low0, High0, Low1, High1),
    Remap(Value.Y, Low0, High0, Low1, High1),
    Remap(Value.Z, Low0, High0, Low1, High1),
  };
}

FRacMath::FCatmullRomSegment FRacMath::ComputeCatmullRom(const FVector& P0, const FVector& P1, const FVector& P2, const FVector& P3, float Alpha, float Tension)
{
  float t0 = 0.0f;
  float t1 = t0 + FMath::Pow(FVector::Distance(P0, P1), Alpha);
  float t2 = t1 + FMath::Pow(FVector::Distance(P1, P2), Alpha);
  float t3 = t2 + FMath::Pow(FVector::Distance(P2, P3), Alpha);

  FVector m1 = (1.0f - Tension) * (t2 - t1) * ((P1 - P0) / (t1 - t0) - (P2 - P0) / (t2 - t0) + (P2 - P1) / (t2 - t1));
  FVector m2 = (1.0f - Tension) * (t2 - t1) * ((P2 - P1) / (t2 - t1) - (P3 - P1) / (t3 - t1) + (P3 - P2) / (t3 - t2));

  return FCatmullRomSegment{
    2.0f * (P1 - P2) + m1 + m2,
    -3.0f * (P1 - P2) - m1 - m1 - m2,
    m1,
    P1,
  };
}

FVector FRacMath::SampleCatmullRom(const FCatmullRomSegment& Segment, float Time)
{
  return FVector{
    Segment.A * Time * Time * Time +
    Segment.B * Time * Time +
    Segment.C * Time +
    Segment.D
  };
}

FVector FRacMath::RandomInsideUnitCircle()
{
  return FMath::FRand() * FRotator(0.0f, FMath::RandRange(0.0f, 360.0f), 0.0f).Vector();
}
FVector FRacMath::RandomInsideUnitSphere()
{
  return FMath::FRand() * FRotator(FMath::RandRange(-90.0f, 90.0f), FMath::FRandRange(0.0f, 360.0f), 0.0f).Vector();
}

FVector FRacMath::RandomInsideRect(const FVector& P, const FVector& S)
{
  return FVector{ FMath::RandRange(P.X, P.X + S.X), FMath::RandRange(P.Y, P.Y + S.Y), FMath::RandRange(P.Z, P.Z + S.Z) };
}

void FRacMath::UniformPointsOnSphere(int N, TArray<FVector>& Points)
{
  float phi = TMathUtilConstants<float>::Pi * (3.0f - FMath::Sqrt(5.0f));
  float theta = 0.0f;
  float radius = 0.0f;

  Points.SetNum(N);

  for (int i = 0; i < N; i++)
  {
    theta = phi * i;
    Points[i].Z = (1.0f - (1.0f / (float)N)) * (1.0f - ((2.0f * i) / ((float)N - 1.0f))) * 1.0f;
    radius = FMath::Sqrt(1.0f - (Points[i].Z * Points[i].Z));
    Points[i].X = radius * FMath::Cos(theta);
    Points[i].Y = radius * FMath::Sin(theta);
  }
}