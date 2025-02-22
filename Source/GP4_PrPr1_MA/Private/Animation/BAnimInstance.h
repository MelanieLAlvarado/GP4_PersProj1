// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class UBAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, meta=(BlueprintThreadSafe))
	FORCEINLINE bool GetIsMoving() const { return Speed > 0.f; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	FORCEINLINE bool GetIsNotMoving() const { return Speed == 0.f; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetSmoothedYawSpeeed() const { return SmoothedYawSpeed; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	bool GetIsFalling() const { return bIsFalling; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	bool GetIsOnGround() const { return !bIsFalling; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetLookYaw() const { return LookRotationDelta.Yaw; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetLookPitch() const { return LookRotationDelta.Pitch; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetYWalkFromForwardDir() const { return WalkYFromForward; }
	
	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	float GetXWalkFromForwardDir() const { return WalkXFromForward; }

	UFUNCTION(BlueprintCallable, meta = (BlueprintThreadSafe))
	FVector GetLastWalkDirection() const { return LastWalkDirection; }

private:
	UPROPERTY()
	class ACharacter* OwnerCharacter;

	UPROPERTY()
	class UCharacterMovementComponent* OwnerCharacterMovementComponent;

	float Speed;

	float YawSpeed;
	float SmoothedYawSpeed;
	bool bIsFalling;

	float WalkYFromForward;
	float WalkXFromForward;

	FVector LastWalkDirection;

	UPROPERTY()
	float YawSpeedSmoothRate = 7.f;

	FRotator PrevBodyRotation;
	FRotator LookRotationDelta;
};
