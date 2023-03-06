#pragma once

#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h"

#include "InputActionValue.h"

#include "RacTPSController.generated.h"

class UInputAction;
class UInputMappingContext;

UCLASS()
class RATCHETANDCLANK_API ARacTPSController : public APlayerController
{
	GENERATED_BODY()

  ////////////////////////////////////////////////////////////////////////
  // Constructor
  ////////////////////////////////////////////////////////////////////////

public:

  ARacTPSController();

  ////////////////////////////////////////////////////////////////////////
  // Controller Overrides
  ////////////////////////////////////////////////////////////////////////

protected:

  virtual void SetupInputComponent() override;

  ////////////////////////////////////////////////////////////////////////
  // Input System
  ////////////////////////////////////////////////////////////////////////

private:

  UInputAction* mInputActionMove = nullptr;
  UInputAction* mInputActionLook = nullptr;
  UInputAction* mInputActionRun = nullptr;
  UInputAction* mInputActionJump = nullptr;
  UInputAction* mInputActionAim = nullptr;
  UInputAction* mInputActionFire = nullptr;
  UInputAction* mInputActionSwap = nullptr;

  UInputMappingContext* mInputMappingContext = nullptr;

  void InputMoveTriggered(const FInputActionValue& Value);
  void InputLookTriggered(const FInputActionValue& Value);
  void InputRunTriggered(const FInputActionValue& Value);
  void InputRunCompleted(const FInputActionValue& Value);
  void InputJumpStarted(const FInputActionValue& Value);
  void InputJumpCompleted(const FInputActionValue& Value);
  void InputAimTriggered(const FInputActionValue& Value);
  void InputAimCompleted(const FInputActionValue& Value);
  void InputFireStarted(const FInputActionValue& Value);
  void InputFireCompleted(const FInputActionValue& Value);
  void InputSwapStarted(const FInputActionValue& Value);
};