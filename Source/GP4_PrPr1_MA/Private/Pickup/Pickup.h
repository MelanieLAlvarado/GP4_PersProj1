// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/SphereComponent.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Framework/InteractInterface.h"
#include "Engine/DataAsset.h"
#include "Pickup.generated.h"

UCLASS()
class APickup : public AActor, public IInteractInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	virtual void InitializeWithDataAsset();

	virtual void Interact(AActor* InteractingActor) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	bool IsPickupActive() const { return bIsActive; }
	UFUNCTION()
	void SetPickupActive(const bool status) { bIsActive = status; };

	UFUNCTION() void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);
	UFUNCTION() void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

private:
	UFUNCTION()
	void OnPickupCollected(AActor* InteractingActor);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	USphereComponent* SphereCollider;

	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere, Category = "Pickup")
	bool bIsActive{ false };
	UPROPERTY(EditAnywhere, Category = "Pickup")
	UDataAsset* PickupItemClass;
};
