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
	float CameraBoomLength = 1200.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CamBoomYOffset = 0.0f;

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
	class UInputAction* InteractInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);

	void HandleFireInput(const struct FInputActionValue& InputActionValue);
	void HandleInteractInput(const struct FInputActionValue& InputActionValue);

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
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	AWeapon* CurrentWeapon;


};
