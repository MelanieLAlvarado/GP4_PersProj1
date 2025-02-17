// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraShakeBase.h"
#include "RecoilCameraShake.generated.h"

/**
 * 
 */
UCLASS()
class URecoilCameraShake : public UCameraShakeBase
{
	GENERATED_BODY()
	
	
public:
	void CalculateRecoilValue(float RecoilStrength);

private:
	UPROPERTY(VisibleAnywhere, Category = "Recoil")
	float RecoilValue;

	UPROPERTY(VisibleAnywhere, Category = "Recoil")
	float RecoilFrequency;
};
