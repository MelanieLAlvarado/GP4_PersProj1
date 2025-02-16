// Fill out your copyright notice in the Description page of Project Settings.

#include "CoreMinimal.h"
#include "Weapon/WeaponComponent.h"

// Sets default values for this component's properties
UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...

}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UWeaponComponent::SetWeaponInfoWidget(UWeaponInfoWidget* WeaponWidget)
{
	WeaponInfoWidget = WeaponWidget;
	if (WeaponInfoWidget)
	{
		OnWeaponUpdated.AddUObject(WeaponInfoWidget, &UWeaponInfoWidget::WeaponUpdated);
	}
}

void UWeaponComponent::SetCurrentWeapon(AWeaponPickup* ReceivedWeaponPickup)
{
	WeaponPickupClass = ReceivedWeaponPickup->GetClass();
	WeaponData = ReceivedWeaponPickup->GetWeaponData();
	CurrentAmmo = ReceivedWeaponPickup->GetCurrentAmmo();
	UpdateWeaponWidget();
}

void UWeaponComponent::UpdateWeaponWidget()
{
	OnWeaponUpdated.Broadcast(WeaponData, CurrentAmmo);
}

bool UWeaponComponent::TryFireWeapon()
{
	UE_LOG(LogTemp, Warning, TEXT("Try Fire..."));
	//UE_LOG(LogTemp, Warning, TEXT("WD: %s"), WeaponData->GetName());
	if (!bCanFire || !WeaponData)
	{
		UE_LOG(LogTemp, Warning, TEXT("failed first if"));
		return false;
	}

	if (CurrentAmmo <= 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Ammo!"));
		return false;
	}

	if (CurrentAmmo > WeaponData->GetMaxAmmo())
		CurrentAmmo = WeaponData->GetMaxAmmo();

	UE_LOG(LogTemp, Warning, TEXT("Try Fire successful"));

	//bCanFire = false;

	CurrentAmmo--;
	UpdateWeaponWidget();
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
	return true;
}

void UWeaponComponent::ReloadWeapon()
{
	if (!WeaponData)
		return;

	if (CurrentAmmo == WeaponData->GetMaxAmmo())
		return;

	CurrentAmmo = WeaponData->GetMaxAmmo();
	UpdateWeaponWidget();
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
}

void UWeaponComponent::TryDropCurrentWeapon(float DropSpawnDistance)
{
	FActorSpawnParameters ActorSpawnParams;

	ActorSpawnParams.SpawnCollisionHandlingOverride;

	AActor* owner = GetOwner();

	const FVector SpawnLocation = owner->GetActorLocation() + (owner->GetActorForwardVector() * DropSpawnDistance);

	AWeaponPickup* PickupActor = GetWorld()->SpawnActor<AWeaponPickup>(WeaponPickupClass, SpawnLocation, FRotator::ZeroRotator);

	if (PickupActor != NULL)
	{
		PickupActor->InitializeWithDataAsset();
		//update current ammo count too
		PickupActor->SetCurrentAmmo(CurrentAmmo);
		CurrentAmmo = 0;
		WeaponData = NULL;
		UpdateWeaponWidget();
	}
}
