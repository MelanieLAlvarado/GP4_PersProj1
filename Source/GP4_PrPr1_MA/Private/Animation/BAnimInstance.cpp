// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/BAnimInstance.h"
#include "Gameframework/Character.h"
#include "Gameframework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());
	if (OwnerCharacter)
	{
		OwnerCharacterMovementComponent = OwnerCharacter->GetCharacterMovement();
	}
}

void UBAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (OwnerCharacter)
	{
		Speed = OwnerCharacter->GetVelocity().Length();

		FRotator BodyRotation = OwnerCharacter->GetActorRotation();
		FRotator RotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(BodyRotation, PrevBodyRotation);

		PrevBodyRotation = BodyRotation;
		YawSpeed = RotationDelta.Yaw / DeltaSeconds;

		SmoothedYawSpeed = UKismetMathLibrary::FInterpTo(SmoothedYawSpeed, YawSpeed, DeltaSeconds, YawSpeedSmoothRate);

		FRotator AimRotation = OwnerCharacter->GetBaseAimRotation();
		LookRotationDelta = UKismetMathLibrary::NormalizedDeltaRotator(AimRotation, BodyRotation);


		FVector RawVelocity = OwnerCharacter->GetVelocity();
		FVector OwnerForward = OwnerCharacter->GetActorForwardVector();
		FVector OwnerRight = OwnerCharacter->GetActorRightVector();

		RawVelocity.Normalize();

		WalkYFromForward = FVector::DotProduct(OwnerForward, RawVelocity);
		WalkXFromForward = FVector::DotProduct(OwnerRight, RawVelocity);

		if (Speed > 0)
		{
			LastWalkDirection = FVector(WalkXFromForward, WalkYFromForward, 0);
		}
	}
	if (OwnerCharacterMovementComponent)
	{
		bIsFalling = OwnerCharacterMovementComponent->IsFalling();
	}
}
