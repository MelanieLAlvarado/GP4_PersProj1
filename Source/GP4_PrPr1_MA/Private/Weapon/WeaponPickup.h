// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pickup/Pickup.h"
#include "WeaponDataAsset.h"
#include "WeaponPickup.generated.h"

/**
 * 
 */
UCLASS()
class AWeaponPickup : public APickup
{
	GENERATED_BODY()


public:
	virtual void BeginPlay() override;

	virtual void InitializeWithDataAsset() override;

	UFUNCTION()
	int GetCurrentAmmo() { return CurrentAmmo; };
	UFUNCTION()
	void SetCurrentAmmo(int NewCurrentAmmo) { CurrentAmmo = NewCurrentAmmo; };

	UFUNCTION()
	UWeaponDataAsset* GetWeaponData() { return WeaponData; }

	UFUNCTION()
	void DisableGravityAfterTimer();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float GravityTimerDuration = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UWeaponDataAsset* WeaponData;
};
