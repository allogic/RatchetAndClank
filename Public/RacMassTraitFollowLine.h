#pragma once

#include "CoreMinimal.h"

#include "MassEntityTraitBase.h"
#include "MassEntityTemplateRegistry.h"

#include "RacMassTraitFollowLine.generated.h"

UCLASS()
class RATCHETANDCLANK_API URacMassTraitFollowLine : public UMassEntityTraitBase
{
	GENERATED_BODY()

	////////////////////////////////////////////////////////////////////////
	// Trait Overrides
	////////////////////////////////////////////////////////////////////////

protected:

	virtual void BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const override;
};