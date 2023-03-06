#include "RacGameMode.h"
#include "RacTPSCharacterThief.h"
#include "RacTPSController.h"
#include "RacHUD.h"

ARacGameMode::ARacGameMode()
{
  DefaultPawnClass = ARacTPSCharacterThief::StaticClass();
  PlayerControllerClass = ARacTPSController::StaticClass();
  HUDClass = ARacHUD::StaticClass();
}