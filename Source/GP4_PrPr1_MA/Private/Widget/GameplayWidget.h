// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameplayWidget.generated.h"

/**
 * 
 */
UCLASS()
class UGameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	
	UFUNCTION()
	UWeaponInfoWidget* GetWeaponInfoWidget() { return WeaponWidget; };

private:
	UPROPERTY(meta = (BindWidget))
	class UWeaponInfoWidget* WeaponWidget;

};
