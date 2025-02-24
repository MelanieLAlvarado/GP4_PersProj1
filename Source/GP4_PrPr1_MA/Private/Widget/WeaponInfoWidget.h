// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponPickup.h"
#include "WeaponInfoWidget.generated.h"

/**
 * 
 *///
UCLASS()
class UWeaponInfoWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativePreConstruct() override;

	void WeaponUpdated(UWeaponDataAsset* WeaponData, int CurrentAmmoValue);

private:
	void ResetValue();
	void SetValue(int NewValue, int NewMaxValue);

	UPROPERTY(meta = (BindWidget))
	class UImage* Icon;

	UPROPERTY(meta=(BindWidget))
	class UTextBlock* ValueText;
};
