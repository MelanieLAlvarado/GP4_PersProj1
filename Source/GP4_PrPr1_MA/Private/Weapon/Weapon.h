// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/Image.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void FireWeapon();

	void Reload();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float DamageToDeal;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float MaxAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	float CurrentAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	UImage* WeaponIcon;

};
