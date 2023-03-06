#include "RacMassTraitFollowLine.h"
#include "RacMassFragments.h"

void URacMassTraitFollowLine::BuildTemplate(FMassEntityTemplateBuildContext& BuildContext, const UWorld& World) const
{
  BuildContext.AddFragment<FRacMassFragmentFollower>();
}