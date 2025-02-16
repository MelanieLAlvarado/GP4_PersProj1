// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/BCharacter.h"
#include "Components/AdsComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Weapon/WeaponPickup.h"
#include "Widget/GameplayWidget.h"
#include "BPlayerCharacter.generated.h"

//DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponUpdated, int /*maxCount*/, int);
//DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponUpdatedDynamic, int, MaxCount, int, Value);

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
	
	//FOnWeaponUpdated OnWeaponUpdated;
	//void WeponUpdated(int a, int b);

	//UFUNCTION()
	//void WeaponUpdatedDynamic(int a, int b);

	//FOnWeaponUpdatedDynamic OnWeaponUpdatedDynamic;

	UFUNCTION()
	void InitializeWeaponComponentUI(UGameplayWidget* GameplayWidget);

	UFUNCTION()
	UCameraComponent* GetViewCamera();

	UFUNCTION()
	USpringArmComponent* GetCameraBoom();

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
	
	UPROPERTY(VisibleDefaultsOnly, Category = "View")
	class UAdsComponent* AdsComponent;

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
	class UInputAction* DropCurrentInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* EndAimInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputAction* ReloadInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	class UInputMappingContext* GameplayInputMappingContext;

	void HandleLookInput(const struct FInputActionValue& InputActionValue);
	void HandleMoveInput(const struct FInputActionValue& InputActionValue);

	void HandleFireInput(const struct FInputActionValue& InputActionValue);
	void HandleInteractInput(const struct FInputActionValue& InputActionValue);

	void HandleDropInput(const struct FInputActionValue& InputActionValue);

	void HandleAimInputHold(const struct FInputActionValue& InputActionValue);
	void HandleAimInputReleased(const struct FInputActionValue& InputActionValue);

	void HandleReloadInput(const struct FInputActionValue& InputActionValue);


	FVector GetLookRightDirection() const;

	FVector GetLookForwardDirection() const;

	FVector GetMoveForwardDirection() const;

	/********************
	*		Interact	*
	*********************/
public:
	UFUNCTION()
	bool TryCanPickup(APickup* PickupClass);
private: 
	UPROPERTY(VisibleAnywhere, Category = "Interact")
	AActor* Interactable;

	/********************
	*		Weapon		*
	*********************/
	
	UPROPERTY(EditAnywhere, Category = "Weapon")
	UWeaponComponent* WeaponComponent;

	bool CalculateFireResult(FHitResult HitResultToCheck);

};
