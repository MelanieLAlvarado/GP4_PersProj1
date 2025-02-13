// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Pickup/Pickup.h"
#include "WeaponDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void UpdateWeaponWidget(APickup* ParentPickup);

	UFUNCTION()
	bool TryFireWeapon();
	UFUNCTION()
	void ReloadWeapon();

	UFUNCTION()
	float GetDamageToDeal() { return DamageToDeal; };

	UFUNCTION()
	int GetMaxAmmo() { return MaxAmmo; };

	UFUNCTION()
	int GetCurrentAmmo() { return CurrentAmmo; };

	UFUNCTION()
	UImage* GetWeaponIcon() { return WeaponIcon; };

	UFUNCTION()
	UStaticMeshComponent* GetStaticMesh() { return PickupMesh; };

protected:
	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool bCanFire;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DamageToDeal;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UImage* WeaponIcon;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;
};
