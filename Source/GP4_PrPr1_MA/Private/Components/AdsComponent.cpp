// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AdsComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BPlayerCharacter.h"

// Sets default values for this component's properties
UAdsComponent::UAdsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	if (!GetOwner())
		return;

	OwnerPlayerCharacter = Cast<ABPlayerCharacter>(GetOwner());
	if (!OwnerPlayerCharacter)
		return;

	ViewCam = OwnerPlayerCharacter->GetViewCamera();
	if (ViewCam)
	{
		ViewCam->SetFieldOfView(DefaultFOV);
	}
	CameraBoom = OwnerPlayerCharacter->GetCameraBoom();
}


// Called when the game starts
void UAdsComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAdsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	ProcessCameraLerp(DeltaTime);
}
void UAdsComponent::SetIsAimingState(bool StateToSet)
{
	bIsAiming = StateToSet;
	bIsViewLerp = true;
}

void UAdsComponent::ProcessCameraLerp(float DeltaTime)
{
	if (!bIsViewLerp)
	{
		return;
	}
	LerpAlpha += DeltaTime * AimLerpMultiplier;

	if (bIsAiming)
	{
		LerpCameraBoomLength(CameraBoomDefaultLength, CameraBoomAimLength);
		LerpFOV(DefaultFOV, AimFOV);
		bIsViewLerp = !(CurrentFOV <= AimFOV && CameraBoomCurrentLength <= CameraBoomAimLength);
	}
	else
	{
		LerpCameraBoomLength(CameraBoomAimLength, CameraBoomDefaultLength);
		LerpFOV(AimFOV, DefaultFOV);
		bIsViewLerp = !(CurrentFOV >= DefaultFOV && CameraBoomCurrentLength >= CameraBoomDefaultLength);
	}

	if (!bIsViewLerp)
	{
		LerpAlpha = 0.f;
	}
	UE_LOG(LogTemp, Warning, TEXT("Fov: %f"), CurrentFOV);
	UE_LOG(LogTemp, Warning, TEXT("Boom: %f"), CameraBoomCurrentLength);
}

void UAdsComponent::LerpCameraBoomLength(float StartValue, float EndValue)
{
	CameraBoomCurrentLength = FMath::Lerp(StartValue, EndValue, LerpAlpha);
	CameraBoom->TargetArmLength = CameraBoomCurrentLength;
}

void UAdsComponent::LerpFOV(float StartValue, float EndValue)
{
	CurrentFOV = FMath::Lerp(StartValue, EndValue, LerpAlpha);
	ViewCam->SetFieldOfView(CurrentFOV);
}

