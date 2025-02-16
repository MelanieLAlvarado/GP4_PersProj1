// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponPickup.h"
#include "Widget/WeaponInfoWidget.h"
#include "WeaponComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, UWeaponDataAsset* /*NewCurrentAmmo*/, int /*CurrentAmmo*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	FOnWeaponChanged OnWeaponUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetWeaponInfoWidget(UWeaponInfoWidget* WeaponWidget);

	UFUNCTION()
	void SetCurrentWeapon(AWeaponPickup* RecievedWeaponPickup);

	UFUNCTION()
	void UpdateWeaponWidget();

	UFUNCTION()
	bool TryFireWeapon();
	UFUNCTION()
	void ReloadWeapon();
	UFUNCTION()
	void TryDropCurrentWeapon(float DropSpawnDistance);

	UFUNCTION()
	UWeaponDataAsset* GetCurrentWeaponData() { return WeaponData; };
private:
	UPROPERTY()
	UWeaponInfoWidget* WeaponInfoWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool bCanFire = true;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UWeaponDataAsset* WeaponData;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<class AWeaponPickup> WeaponPickupClass;
};
