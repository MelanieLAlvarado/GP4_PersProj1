// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponPickup.h"

void AWeaponPickup::BeginPlay()
{
	Super::BeginPlay();

	if (PickupItemClass)
		WeaponData = Cast<UWeaponDataAsset>(PickupItemClass);
	
	InitializeWithDataAsset();
}

void AWeaponPickup::InitializeWithDataAsset()
{
	Super::InitializeWithDataAsset();
	if (!WeaponData)
		WeaponData = Cast<UWeaponDataAsset>(PickupItemClass);

	if (WeaponData && WeaponData->GetStaticMesh())
	{
		PickupMesh->SetStaticMesh(WeaponData->GetStaticMesh());
		PickupMesh->SetSimulatePhysics(false);//May change later
		//connect delegates for ui
		//UpdateWeaponWidget();
	}
}

