#pragma once

#include "CoreMinimal.h"

#include "MassProcessor.h"

#include "RacMassProcessorShip.generated.h"

UCLASS()
class RATCHETANDCLANK_API URacMassProcessorShip : public UMassProcessor
{
	GENERATED_BODY()

public:

	URacMassProcessorShip();

protected:

	virtual void ConfigureQueries() override;
	virtual void Execute(FMassEntityManager& EntityManager, FMassExecutionContext& Context) override;

private:

	FMassEntityQuery mEntityQuery = { *this };

	struct FPaths
	{
		FVector Start;
		FVector End;
	};

	TArray<FPaths> mPaths =	{
		{ FVector{ -5000.0f, -5000.0f, 5000.0f }, FVector{ -5000.0f,  5000.0f, 5000.0f } },
		{ FVector{ -5000.0f, -5000.0f, 5000.0f }, FVector{  5000.0f, -5000.0f, 5000.0f } },
		{ FVector{  5000.0f,  5000.0f, 5000.0f }, FVector{ -5000.0f,  5000.0f, 5000.0f } },
		{ FVector{  5000.0f,  5000.0f, 5000.0f }, FVector{  5000.0f, -5000.0f, 5000.0f } },
	};
};
