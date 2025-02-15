// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPickup.h"

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupItemClass)
		WeaponData = Cast<UWeaponDataAsset>(PickupItemClass);
}

void AWeaponPickup::InitializeWithDataAsset()
{
	Super::InitializeWithDataAsset();
	if (!WeaponData)
		WeaponData = Cast<UWeaponDataAsset>(PickupItemClass);

	if (WeaponData)
	{
		PickupMesh = WeaponData->GetStaticMesh();
		//connect delegates for ui
		//UpdateWeaponWidget();
		return;
	}
}

void AWeaponPickup::UpdateWeaponWidget()
{
	//use connected delegates to send all weaponData ui info
}

bool AWeaponPickup::TryFireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Fire..."));
	UE_LOG(LogTemp, Warning, TEXT("WD: %s"), WeaponData->GetName());
	if (!bCanFire || !WeaponData)
		UE_LOG(LogTemp, Warning, TEXT("failed first if"));
		return false;

	if (CurrentAmmo <= 0)
		UE_LOG(LogTemp, Warning, TEXT("failed second if"));
		return false;

	if (CurrentAmmo > WeaponData->GetMaxAmmo())
		CurrentAmmo = WeaponData->GetMaxAmmo();

	UE_LOG(LogTemp, Warning, TEXT("Try Fire successful"));

	//bCanFire = false;

	CurrentAmmo--;
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
	return true;
}

void AWeaponPickup::ReloadWeapon()
{
	if (!WeaponData)
		return;

	if (CurrentAmmo == WeaponData->GetMaxAmmo())
		return;

	CurrentAmmo = WeaponData->GetMaxAmmo();
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
}