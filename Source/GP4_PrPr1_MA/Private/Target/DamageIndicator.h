// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/WidgetComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageIndicator.generated.h"

UCLASS()
class ADamageIndicator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADamageIndicator();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void InitializeDamageIndicator(float DamageToDisplay);

private:
	UPROPERTY()
	APlayerController* InstigatorController;

	UPROPERTY(EditAnywhere, Category = "Damage Indicator")
	UWidgetComponent* WidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	UUserWidget* DamageIndicatorWidget;

	//
	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float MoveUpSpeed = 2.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float TimerDuration = 1.5f;

	FTimerHandle TimerHandle;

	UFUNCTION()
	void TimerEnd();

	UPROPERTY(EditDefaultsOnly, Category = "Damage Indicator")
	float DestroyDelay = 3.0f;
};
