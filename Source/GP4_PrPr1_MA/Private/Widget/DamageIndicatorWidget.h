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
	virtual void NativeConstruct() override;

	UFUNCTION()
	void DisplayDamage(float DamageDealt);

private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ValueText;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float MoveUpSpeed = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float TimerDuration = 3.0f;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void TimerEnd();

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float DestroyDelay = 3.0f;

	UPROPERTY()
	bool bDestroyCountDownStarted = false;

};
