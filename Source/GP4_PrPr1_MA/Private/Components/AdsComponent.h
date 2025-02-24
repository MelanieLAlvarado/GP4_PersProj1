// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "AdsComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UAdsComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAdsComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetIsAimingState(bool StateToSet);

private:
	UPROPERTY()
	class ABPlayerCharacter* OwnerPlayerCharacter;

	UPROPERTY()
	class UCameraComponent* ViewCam;

	UPROPERTY()
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	UCurveFloat* AdsCurve;

	UPROPERTY()
	FTimeline AdsTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	float CameraBoomDefaultLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	float CameraBoomAimLength = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "Aim Down Sights")
	float CameraBoomCurrentLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	float DefaultFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	float AimFOV = 80.f;

	UPROPERTY(EditDefaultsOnly, Category = "Aim Down Sights")
	float CurrentFOV = 90.f;

	void LerpCameraBoomLength(float StartValue, float EndValue, float Value);
	void LerpFOV(float StartValue, float EndValue, float Value);

	UPROPERTY()
	bool bIsViewLerp = false;
	UPROPERTY()
	bool bIsAiming = false;

	UFUNCTION()
	void UpdateCameraLerp(float Value);
};
