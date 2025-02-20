// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DamageIndicatorWidget.generated.h"

/**
 * 
 */
UCLASS()
class UDamageIndicatorWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void DisplayDamage(float DamageDealt);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ValueText;
};
