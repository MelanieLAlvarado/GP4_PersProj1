// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/WeaponDataAsset.h"
#include "Components/ActorComponent.h"
#include "TargetComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTargetComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void HitAction(UWeaponDataAsset* WeaponHitData, FHitResult HitResult);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Target")
	TSubclassOf<class ADamageIndicator> DamageIndicatorClass;
};
