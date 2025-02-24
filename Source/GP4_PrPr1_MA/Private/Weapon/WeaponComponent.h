// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Camera/CameraComponent.h"
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "Weapon/WeaponDataAsset.h"
#include "Weapon/WeaponPickup.h"
#include "Widget/WeaponInfoWidget.h"
#include "WeaponComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_TwoParams(FOnWeaponChanged, UWeaponDataAsset* /*NewCurrentAmmo*/, int /*CurrentAmmo*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UWeaponComponent();

	FOnWeaponChanged OnWeaponUpdated;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void SetWeaponInfoWidget(UWeaponInfoWidget* WeaponWidget);

	UFUNCTION()
	void SetCurrentWeapon(AWeaponPickup* RecievedWeaponPickup);

	UFUNCTION()
	void UpdateWeaponWidget();

	UFUNCTION()
	bool TryFireWeapon(UCameraComponent* ViewCam, ECollisionChannel TargetChannel);
	UFUNCTION()
	void ProcessCanFire();
	UFUNCTION()
	void ReloadWeapon();
	UFUNCTION()
	void TryDropCurrentWeapon();

	UFUNCTION()
	UWeaponDataAsset* GetCurrentWeaponData() { return WeaponData; };

private:
	ACharacter* OwnerCharacter;
	/************************************
	*		Line Trace/ Fire Weapon		*
	*************************************/
	UFUNCTION()
	bool CalculateFireResult(UCameraComponent* ViewCam, ECollisionChannel TargetChannel, FHitResult& HitResult);

	UFUNCTION()
	void ProcessHitActor(FHitResult HitResult);

	/********************
	*		Recoil		*
	*********************/

	UFUNCTION()
	void StartRecoil();

	UFUNCTION()
	void ProcessRecoil(float RawValue);

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	TSubclassOf<UCameraShakeBase> FireCamShake;

	UPROPERTY()
	FTimeline RecoilTimeline;

	UPROPERTY(EditDefaultsOnly, Category = "Recoil")
	UCurveFloat* RecoilCurve;
	bool bIsTimelinePlaying;

	UPROPERTY()
	FTimerHandle EndRecoilTimerHandle;
	UFUNCTION()
	void EndRecoil();


	/********************************
	*		Weapon Info/Data		*
	*********************************/

	UPROPERTY()
	UWeaponInfoWidget* WeaponInfoWidget;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	FName AttachWeaponSocketName;

	UPROPERTY()
	AActor* HeldWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	float DropSpawnDistance = 125.f;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	bool bCanFire = true;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	UWeaponDataAsset* WeaponData;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	TSubclassOf<class AWeaponPickup> WeaponPickupClass;

	UFUNCTION()
	void AttachWeaponToSocket();

	/************************
	*		Animations		*
	*************************/
private:
	UAnimInstance* AnimInstance;

	void PlayAnimMontage(UAnimMontage* AnimMontageToPlay);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ShootMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	UAnimMontage* ShootNoReloadMontage;
};
