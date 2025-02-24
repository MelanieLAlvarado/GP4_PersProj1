// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/AdsComponent.h"
#include "Components/TimelineComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/BPlayerCharacter.h"

#define GET_FUNCTION_NAME(function) #function



// Sets default values for this component's properties
UAdsComponent::UAdsComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...

}


// Called when the game starts
void UAdsComponent::BeginPlay()
{
	Super::BeginPlay();

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

	if (AdsCurve)
	{
		FString UpdateCameraLerpName = GET_FUNCTION_NAME(UpdateCameraLerp);

		FOnTimelineFloat TimelineCallback;
		TimelineCallback.BindUFunction(this, FName(UpdateCameraLerpName));
		AdsTimeline.AddInterpFloat(AdsCurve, TimelineCallback);

		UE_LOG(LogTemp, Warning, TEXT("Function: %s"), *UpdateCameraLerpName);
	}
}


// Called every frame
void UAdsComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (AdsTimeline.IsPlaying())
	{
		AdsTimeline.TickTimeline(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("TimeVal: %f"), AdsTimeline.GetPlaybackPosition());
	}
}

void UAdsComponent::SetIsAimingState(bool StateToSet)
{//
	UE_LOG(LogTemp, Warning, TEXT("%s"), StateToSet ? TEXT("True") : TEXT("False"));
	bIsAiming = StateToSet;

	bIsViewLerp = true;
	if (bIsAiming)
	{
		AdsTimeline.Play();
	}
	else
	{
		AdsTimeline.Play();
		AdsTimeline.Reverse();
	}
}

void UAdsComponent::LerpCameraBoomLength(float StartValue, float EndValue, float Value)
{
	CameraBoomCurrentLength = FMath::Lerp(StartValue, EndValue, Value);
	CameraBoom->TargetArmLength = CameraBoomCurrentLength;
}

void UAdsComponent::LerpFOV(float StartValue, float EndValue, float Value)
{
	CurrentFOV = FMath::Lerp(StartValue, EndValue, Value);
	ViewCam->SetFieldOfView(CurrentFOV);
}

void UAdsComponent::UpdateCameraLerp(float Value)
{
	UE_LOG(LogTemp, Warning, TEXT("alpha: %f"), Value);
	LerpCameraBoomLength(CameraBoomDefaultLength, CameraBoomAimLength, Value);
	LerpFOV(DefaultFOV, AimFOV, Value);
}

