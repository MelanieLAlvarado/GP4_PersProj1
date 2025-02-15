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
	void UpdateWeaponWidget();

	UFUNCTION()
	int GetCurrentAmmo() { return CurrentAmmo; };

	UFUNCTION()
	UWeaponDataAsset* GetWeaponData() { return WeaponData; }

	UFUNCTION()
	bool TryFireWeapon();
	UFUNCTION()
	void ReloadWeapon();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool bCanFire = true;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UWeaponDataAsset* WeaponData;
};
