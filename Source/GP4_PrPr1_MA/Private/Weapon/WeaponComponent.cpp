// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponComponent.h"
#include "Components/TargetComponent.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"

#define GET_FUNCTION_NAME(function) #function

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
	OwnerPawn = Cast<APawn>(GetOwner());
	if (FloatCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FString ProcessRecoilName = GET_FUNCTION_NAME(ProcessRecoil);
		TimelineCallback.BindUFunction(this, FName(ProcessRecoilName));
		RecoilTimeline.AddInterpFloat(FloatCurve, TimelineCallback);
	}
}


// Called every frame
void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	if (bIsTimelinePlaying)
	{
		RecoilTimeline.TickTimeline(DeltaTime);
	}
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

bool UWeaponComponent::TryFireWeapon(FHitResult HitResult, UCameraComponent* ViewCam, ECollisionChannel TargetChannel)
{
	if (!bCanFire || !WeaponData)
		return false;

	if (CurrentAmmo <= 0)
		return false;

	if (CurrentAmmo > WeaponData->GetMaxAmmo())
		CurrentAmmo = WeaponData->GetMaxAmmo();

	if (!CalculateFireResult(ViewCam, TargetChannel, HitResult))
	{
		return false;
	}

	CurrentAmmo--;
	ProcessHitActor(HitResult);
	StartRecoil();
	UpdateWeaponWidget();
	UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
	return true;
}
bool UWeaponComponent::CalculateFireResult(UCameraComponent* ViewCam, ECollisionChannel TargetChannel, FHitResult& HitResult)
{
	float FireDistance = WeaponData->GetWeaponRange();//should move to either the 

	const FVector CamLineStart = ViewCam->GetComponentLocation();
	const FVector CamLineEnd = CamLineStart + ViewCam->GetForwardVector() * FireDistance;
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, CamLineStart, CamLineEnd, TargetChannel))
	{//calculating from camera straight onwards
		return false;
	}
	DrawDebugLine(GetWorld(), CamLineStart, CamLineEnd, FColor::Blue, true, 3.f);

	const FVector OwnerLineStart = GetOwner()->GetActorLocation();
	const FVector OwnerLineEnd = HitResult.ImpactPoint;

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, OwnerLineStart, OwnerLineEnd, TargetChannel))
	{//calculating from player to camera hit line
		return false;
	}
	DrawDebugLine(GetWorld(), OwnerLineEnd, OwnerLineStart, FColor::Red, true, 3.f);
	return true;
}

void UWeaponComponent::ProcessHitActor(FHitResult HitResult)
{
	AActor* HitActor = HitResult.GetActor();

	if (!IsValid(HitActor))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Hit Actor. "));
		return;
	}

	UTargetComponent* Target = Cast<UTargetComponent>(HitActor->GetComponentByClass(UTargetComponent::StaticClass()));
	if (!Target)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Target Script"));
		return;
	}
	Target->HitAction(WeaponData, HitResult);
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
	GetWorld()->GetTimerManager().SetTimer(EndRecoilTimerHandle, this, &UWeaponComponent::EndRecoil, 0.2f, false);

	bIsTimelinePlaying = true;
	RecoilTimeline.PlayFromStart();
}

void UWeaponComponent::ProcessRecoil(float RawValue)
{
	if (OwnerPawn && FloatCurve)
	{
		UE_LOG(LogTemp, Warning, TEXT("RawVal: %f"), RawValue);
		float RecoilValue = WeaponData->GetRecoilStrength() * RawValue;
		OwnerPawn->AddControllerPitchInput(-RecoilValue);
	}
	return;
}

void UWeaponComponent::EndRecoil()
{
	GetWorld()->GetTimerManager().ClearTimer(EndRecoilTimerHandle);

	bIsTimelinePlaying = false;
	RecoilTimeline.Stop();
}
