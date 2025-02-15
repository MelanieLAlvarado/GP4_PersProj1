// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponPickup.h"
#include "WeaponInfoWidget.generated.h"

/**
 * 
 *///DECLARE_MULTICAST_DELEGATE_OneParam(FOnWeaponStatusChanged, AWeaponDataAsset /*NewCurrentAmmo*/);
UCLASS()
class UWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;
	void InitializeWithCurrentWeapon(AWeaponPickup* WeaponPickup);
	void UpdateWithCurrentWeapon(AWeaponPickup* WeaponPickup);

	/*UPROPERTY(BlueprintAssignable)
	FOnWeaponStatusChanged OnWeaponAmmoChanged;*/

private:

	void SetValue(int NewValue, int NewMaxValue);

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ValueText;

	void ValueChanged(AWeaponPickup* WeaponPickup);
	void MaxValueChanged(AWeaponPickup* WeaponPickup);

	float CachedValue;
	float CachedMaxValue;
};
