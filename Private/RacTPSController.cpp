#include "RacTPSController.h"
#include "RacTPSCharacterBase.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInput/Public/InputAction.h"
#include "EnhancedInput/Public/InputMappingContext.h"

#include "Camera/CameraComponent.h"

ARacTPSController::ARacTPSController()
{
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionMove(TEXT("InputAction'/Game/RatchetAndClank/Input/Move.Move'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionLook(TEXT("InputAction'/Game/RatchetAndClank/Input/Look.Look'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionRun(TEXT("InputAction'/Game/RatchetAndClank/Input/Run.Run'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionJump(TEXT("InputAction'/Game/RatchetAndClank/Input/Jump.Jump'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionAim(TEXT("InputAction'/Game/RatchetAndClank/Input/Aim.Aim'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionFire(TEXT("InputAction'/Game/RatchetAndClank/Input/Fire.Fire'"));
  static ConstructorHelpers::FObjectFinder<UInputAction> inputActionSwap(TEXT("InputAction'/Game/RatchetAndClank/Input/Swap.Swap'"));

  static ConstructorHelpers::FObjectFinder<UInputMappingContext> inputMappingContext(TEXT("InputMappingContext'/Game/RatchetAndClank/Input/Context.Context'"));

  if (inputActionMove.Succeeded()) mInputActionMove = inputActionMove.Object;
  if (inputActionLook.Succeeded()) mInputActionLook = inputActionLook.Object;
  if (inputActionRun.Succeeded()) mInputActionRun = inputActionRun.Object;
  if (inputActionJump.Succeeded()) mInputActionJump = inputActionJump.Object;
  if (inputActionAim.Succeeded()) mInputActionAim = inputActionAim.Object;
  if (inputActionFire.Succeeded()) mInputActionFire = inputActionFire.Object;
  if (inputActionSwap.Succeeded()) mInputActionSwap = inputActionSwap.Object;

  if (inputMappingContext.Succeeded()) mInputMappingContext = inputMappingContext.Object;
}

void ARacTPSController::SetupInputComponent()
{
  Super::SetupInputComponent();

  if (UEnhancedInputLocalPlayerSubsystem* playerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
  {
    playerSubsystem->ClearAllMappings();
    playerSubsystem->AddMappingContext(mInputMappingContext, 0);
  }

  if (UEnhancedInputComponent* playerEnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
  {
    playerEnhancedInputComponent->BindAction(mInputActionMove, ETriggerEvent::Triggered, this, &ARacTPSController::InputMoveTriggered);
    playerEnhancedInputComponent->BindAction(mInputActionLook, ETriggerEvent::Triggered, this, &ARacTPSController::InputLookTriggered);
    playerEnhancedInputComponent->BindAction(mInputActionRun, ETriggerEvent::Triggered, this, &ARacTPSController::InputRunTriggered);
    playerEnhancedInputComponent->BindAction(mInputActionRun, ETriggerEvent::Completed, this, &ARacTPSController::InputRunCompleted);
    playerEnhancedInputComponent->BindAction(mInputActionJump, ETriggerEvent::Started, this, &ARacTPSController::InputJumpStarted);
    playerEnhancedInputComponent->BindAction(mInputActionJump, ETriggerEvent::Completed, this, &ARacTPSController::InputJumpCompleted);
    playerEnhancedInputComponent->BindAction(mInputActionAim, ETriggerEvent::Triggered, this, &ARacTPSController::InputAimTriggered);
    playerEnhancedInputComponent->BindAction(mInputActionAim, ETriggerEvent::Completed, this, &ARacTPSController::InputAimCompleted);
    playerEnhancedInputComponent->BindAction(mInputActionFire, ETriggerEvent::Started, this, &ARacTPSController::InputFireStarted);
    playerEnhancedInputComponent->BindAction(mInputActionFire, ETriggerEvent::Completed, this, &ARacTPSController::InputFireCompleted);
    playerEnhancedInputComponent->BindAction(mInputActionSwap, ETriggerEvent::Started, this, &ARacTPSController::InputSwapStarted);
  }
}

void ARacTPSController::InputMoveTriggered(const FInputActionValue& Value)
{
  FVector2D value = Value.Get<FVector2D>();

  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->AddMovementInput(character->GetActorRightVector(), value.X);
    character->AddMovementInput(character->GetActorForwardVector(), value.Y);
  }
}

void ARacTPSController::InputLookTriggered(const FInputActionValue& Value)
{
  FVector2D value = Value.Get<FVector2D>();

  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    FRotator cameraRotation = character->GetComponentCamera()->GetRelativeRotation();

    float pitch = character->GetCameraInvertYaw() ? -value.Y : value.Y;
    float yaw = character->GetCameraInvertPitch() ? -value.X : value.X;

    cameraRotation.Pitch = FMath::Clamp(cameraRotation.Pitch + pitch, character->GetCameraPitchMin(), character->GetCameraPitchMax());

    character->GetComponentCamera()->SetRelativeRotation(cameraRotation);

    character->AddControllerYawInput(yaw);
  }
}

void ARacTPSController::InputRunTriggered(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetLocomotionRunning(true);
  }
}

void ARacTPSController::InputRunCompleted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetLocomotionRunning(false);
  }
}

void ARacTPSController::InputJumpStarted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetLocomotionJumping(true);
  }
}

void ARacTPSController::InputJumpCompleted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetLocomotionJumping(false);
  }
}

void ARacTPSController::InputAimTriggered(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetWeaponAiming(true);
  }
}

void ARacTPSController::InputAimCompleted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetWeaponAiming(false);
  }
}

void ARacTPSController::InputFireStarted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetWeaponFiring(true);
  }
}

void ARacTPSController::InputFireCompleted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->SetWeaponFiring(false);
  }
}

void ARacTPSController::InputSwapStarted(const FInputActionValue& Value)
{
  if (ARacTPSCharacterBase* character = Cast<ARacTPSCharacterBase>(GetCharacter()))
  {
    character->TriggerWeaponSwap();
  }
}