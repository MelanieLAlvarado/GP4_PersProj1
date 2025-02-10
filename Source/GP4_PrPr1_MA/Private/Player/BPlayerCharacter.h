// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BCharacter.h"
#include "Weapon/Weapon.h"
#include "BPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class ABPlayerCharacter : public ABCharacter
{
	GENERATED_BODY()
public:
	ABPlayerCharacter();
	virtual void PawnClientRestart() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Tick(float DeltaTime) override;


	UFUNCTION()
	void SetInteractable(AActor* InteractableToSet);

private:
	/********************
	*		Camera		*
	*********************/

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class UCameraComponent* ViewCam;

	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CamBoomYOffset = 0.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CameraBoomDefaultLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CameraBoomAimLength = 400.f;

	UPROPERTY(VisibleAnywhere, Category = "View")
	float CameraBoomCurrentLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float DefaultFOV = 90.f;
	
	UPROPERTY(EditDefaultsOnly, Category = "View")
	float AimFOV = 70.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CurrentFOV = 90.f;

	float LerpAlpha = 0.f;

	void LerpCameraBoomLength(float StartValue, float EndValue);
	void LerpFOV(float StartValue, float EndValue);
	
	UPROPERTY();
	bool bIsViewLerp = false;
	UPROPERTY();
	bool bIsAiming = false;

	/********************
	*		Input		*
	*********************/

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* FireInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* AimInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* EndAimInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);

	void HandleFireInput(const struct FInputActionValue& InputActionValue);
	void HandleInteractInput(const struct FInputActionValue& InputActionValue);

	void HandleAimInputHold(const struct FInputActionValue& InputActionValue);
	void HandleAimInputReleased(const struct FInputActionValue& InputActionValue);

	FVector GetLookRightDirection() const;

	FVector GetLookForwardDirection() const;

	FVector GetMoveForwardDirection() const;

	/********************
	*		Interact	*
	*********************/
public:
	UFUNCTION()
	bool TryCanPickup(TSubclassOf<class AActor> PickupClass);
private:
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	AActor* Interactable;

	/********************
	*		Weapon		*
	*********************/
	UPROPERTY(EditAnywhere, Category = "Weapon")
	TSubclassOf<AWeapon> CurrentWeapon;


};
