// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponDataAsset.h"
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
	void InitializeWithCurrentWeapon(UWeaponDataAsset* WeaponData);
	void UpdateWithCurrentWeapon(UWeaponDataAsset* WeaponData);

	/*UPROPERTY(BlueprintAssignable)
	FOnWeaponStatusChanged OnWeaponAmmoChanged;*/

private:

	void SetValue(int NewValue, int NewMaxValue);

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ValueText;

	void ValueChanged(UWeaponDataAsset* WeaponData);
	void MaxValueChanged(UWeaponDataAsset* WeaponData);

	float CachedValue;
	float CachedMaxValue;
};
