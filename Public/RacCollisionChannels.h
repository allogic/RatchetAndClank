#pragma once

#include "CoreMinimal.h"

struct ERacCollisionChannel
{
  constexpr static ECollisionChannel ECC_Crate = ECollisionChannel::ECC_GameTraceChannel1;
  constexpr static ECollisionChannel ECC_Bullet = ECollisionChannel::ECC_GameTraceChannel2;
  constexpr static ECollisionChannel ECC_AI = ECollisionChannel::ECC_GameTraceChannel3;
};