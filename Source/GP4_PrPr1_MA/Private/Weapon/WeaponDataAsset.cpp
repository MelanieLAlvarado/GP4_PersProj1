// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponDataAsset.h"


void UWeaponDataAsset::UpdateWeaponWidget(APickup* ParentPickup)
{
}

bool UWeaponDataAsset::TryFireWeapon()
{
	/*if (!bCanFire)
		return;*/

	if (CurrentAmmo <= 0)
		return false;

	if (CurrentAmmo > MaxAmmo)
		CurrentAmmo = MaxAmmo;

	bCanFire = false;
	
	CurrentAmmo--;
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, MaxAmmo);
	return true;
}

void UWeaponDataAsset::ReloadWeapon()
{
	if (CurrentAmmo == MaxAmmo)
		return;

	CurrentAmmo = MaxAmmo;
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, MaxAmmo);
}



