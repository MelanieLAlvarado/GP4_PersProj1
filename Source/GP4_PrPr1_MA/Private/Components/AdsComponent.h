// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CameraBoomDefaultLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CameraBoomAimLength = 300.f;

	UPROPERTY(VisibleAnywhere, Category = "View")
	float CameraBoomCurrentLength = 500.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float DefaultFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float AimFOV = 80.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float CurrentFOV = 90.f;

	float LerpAlpha = 0.f;

	UPROPERTY(EditDefaultsOnly, Category = "View")
	float AimLerpMultiplier = 2.f;

	void ProcessCameraLerp(float DeltaTime);

	void LerpCameraBoomLength(float StartValue, float EndValue);
	void LerpFOV(float StartValue, float EndValue);

	UPROPERTY();
	bool bIsViewLerp = false;
	UPROPERTY();
	bool bIsAiming = false;
};
