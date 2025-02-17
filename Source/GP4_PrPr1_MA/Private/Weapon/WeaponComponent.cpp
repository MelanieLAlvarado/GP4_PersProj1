// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponComponent.h"
#include "CoreMinimal.h"

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
	TryDropCurrentWeapon();

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
	if (!bCanFire || !WeaponData)
		return false;

	if (CurrentAmmo <= 0)
		return false;

	if (CurrentAmmo > WeaponData->GetMaxAmmo())
		CurrentAmmo = WeaponData->GetMaxAmmo();

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

void UWeaponComponent::TryDropCurrentWeapon()
{
	if (!WeaponPickupClass || !WeaponData)
		return;
	FActorSpawnParameters ActorSpawnParams;

	ActorSpawnParams.SpawnCollisionHandlingOverride;

	AActor* owner = GetOwner();

	const FVector SpawnLocation = owner->GetActorLocation() + (owner->GetActorForwardVector() * DropSpawnDistance);

	AWeaponPickup* PickupActor = GetWorld()->SpawnActor<AWeaponPickup>(WeaponPickupClass, SpawnLocation, FRotator::ZeroRotator);

	if (PickupActor != NULL)
	{
		PickupActor->InitializeWithDataAsset();
		//update current ammo count too so asset "remembers"
		PickupActor->SetCurrentAmmo(CurrentAmmo);
		CurrentAmmo = 0;
		WeaponData = NULL;
		UpdateWeaponWidget();
	}
}

void UWeaponComponent::StartRecoil()
{
	GetWorld()->GetTimerManager().ClearTimer(RecoilTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EndRecoilTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(RecoilTimerHandle, this, &UWeaponComponent::ProcessRecoil, 0.01f, true);
	GetWorld()->GetTimerManager().SetTimer(EndRecoilTimerHandle, this, &UWeaponComponent::EndRecoil, 0.1f, false);
}

void UWeaponComponent::ProcessRecoil()
{
	APawn* OwnerCharacter = Cast<APawn>(GetOwner());
	if (OwnerCharacter)
	{
		OwnerCharacter->AddControllerPitchInput(-WeaponData->GetRecoilStrength());
	}
}

void UWeaponComponent::EndRecoil()
{
	GetWorld()->GetTimerManager().ClearTimer(RecoilTimerHandle);
	GetWorld()->GetTimerManager().ClearTimer(EndRecoilTimerHandle);
}
