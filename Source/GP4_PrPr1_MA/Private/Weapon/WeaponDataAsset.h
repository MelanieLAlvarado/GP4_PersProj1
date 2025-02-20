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
	const float GetDamageToDeal() { return DamageToDeal; };
	
	UFUNCTION()
	const float GetRecoilStrength() { return RecoilStrength; };

	UFUNCTION()
	const float GetWeaponRange() { return WeaponRange; };

	UFUNCTION()
	const int GetMaxAmmo() { return MaxAmmo; };

	UFUNCTION()
	UTexture2D* GetWeaponIcon() { return WeaponIconTexture; };

	UFUNCTION()
	UStaticMesh* GetStaticMesh() { return PickupMesh; };

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DamageToDeal;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float RecoilStrength;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float WeaponRange;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UTexture2D* WeaponIconTexture;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UStaticMesh* PickupMesh;
};
