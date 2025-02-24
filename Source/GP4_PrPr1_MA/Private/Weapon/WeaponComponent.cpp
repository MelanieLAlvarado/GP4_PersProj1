// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponComponent.h"
#include "Camera/CameraShakeBase.h"
#include "Components/TargetComponent.h"
#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
#include "Curves/CurveFloat.h"
#include "GameFramework/Character.h"

#define GET_FUNCTION_NAME(function) #function

UWeaponComponent::UWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	//OwnerCharacter = Cast<APawn>(GetOwner());
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		AnimInstance = OwnerCharacter->GetMesh()->GetAnimInstance();
	}

	if (RecoilCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FString ProcessRecoilName = GET_FUNCTION_NAME(ProcessRecoil);
		TimelineCallback.BindUFunction(this, FName(ProcessRecoilName));
		RecoilTimeline.AddInterpFloat(RecoilCurve, TimelineCallback);
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
	AttachWeaponToSocket();

	UpdateWeaponWidget();
}

void UWeaponComponent::UpdateWeaponWidget()
{
	OnWeaponUpdated.Broadcast(WeaponData, CurrentAmmo);
}

bool UWeaponComponent::TryFireWeapon(UCameraComponent* ViewCam, ECollisionChannel TargetChannel)
{
	if (!bCanFire || !WeaponData)
		return false;

	if (CurrentAmmo <= 0)
	{
		PlayAnimMontage(ShootNoReloadMontage);
		return false;
	}

	if (CurrentAmmo > WeaponData->GetMaxAmmo())
		CurrentAmmo = WeaponData->GetMaxAmmo();

	bCanFire = false;

	PlayAnimMontage(ShootMontage);

	CurrentAmmo--;
	StartRecoil();
	UpdateWeaponWidget();

	if (FireCamShake)
	{
		APlayerController* OwnerController = Cast<APlayerController>(OwnerCharacter->GetController());
		if (OwnerController)
		{
			OwnerController->ClientStartCameraShake(FireCamShake);
			UE_LOG(LogTemp, Warning, TEXT("Cam Shake &&& PLayer Controller here"));
		}
	}
	FHitResult HitResult;
	if (!CalculateFireResult(ViewCam, TargetChannel, HitResult))
	{
		return false;
	}

	ProcessHitActor(HitResult);
	//UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
	return true;
}
void UWeaponComponent::ProcessCanFire()
{
	bCanFire = true;
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
	bCanFire = false;

	PlayAnimMontage(ReloadMontage);

	CurrentAmmo = WeaponData->GetMaxAmmo();
	UpdateWeaponWidget();
	//UE_LOG(LogTemp, Warning, TEXT("%i / %i"), CurrentAmmo, WeaponData->GetMaxAmmo());
}

void UWeaponComponent::TryDropCurrentWeapon()
{
	if (!WeaponPickupClass || !WeaponData)
		return;
	FActorSpawnParameters ActorSpawnParams;

	ActorSpawnParams.SpawnCollisionHandlingOverride;

	AActor* owner = GetOwner();

	const FVector SpawnLocation = owner->GetActorLocation() + (owner->GetActorForwardVector() * DropSpawnDistance);

	AWeaponPickup* PickupActor = NULL;
	if (WeaponPickupClass)
	{
		PickupActor = GetWorld()->SpawnActor<AWeaponPickup>(WeaponPickupClass, SpawnLocation, FRotator::ZeroRotator);
	}

	if (PickupActor != NULL)
	{
		if (HeldWeapon)
		{
			HeldWeapon->Destroy();
			HeldWeapon = NULL;
		}

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
	if (OwnerCharacter && RecoilCurve && WeaponData)
	{
		//UE_LOG(LogTemp, Warning, TEXT("RawVal: %f"), RawValue);
		float RecoilValue = WeaponData->GetRecoilStrength() * RawValue;
		OwnerCharacter->AddControllerPitchInput(-RecoilValue);
	}
}

void UWeaponComponent::EndRecoil()
{
	GetWorld()->GetTimerManager().ClearTimer(EndRecoilTimerHandle);

	bIsTimelinePlaying = false;
	RecoilTimeline.Stop();
}

void UWeaponComponent::AttachWeaponToSocket()
{
	if (!OwnerCharacter || !WeaponData || !WeaponData->GetWeaponToAttachHold())
		return;

	if (const USkeletalMeshSocket* AttachWeaponSocket = OwnerCharacter->GetMesh()->GetSocketByName(AttachWeaponSocketName))
	{
		FTransform SocketTransform = OwnerCharacter->GetMesh()->GetSocketTransform(AttachWeaponSocketName);

		HeldWeapon = GetWorld()->SpawnActor<AActor>(WeaponData->GetWeaponToAttachHold(), SocketTransform);

		UStaticMeshComponent* HeldWeaponMesh = HeldWeapon->GetComponentByClass<UStaticMeshComponent>();

		HeldWeaponMesh->SetSimulatePhysics(false);
		HeldWeaponMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		FAttachmentTransformRules WeaponAttachmentRules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, true);

		HeldWeapon->AttachToComponent(OwnerCharacter->GetMesh(), WeaponAttachmentRules, AttachWeaponSocketName);
		//UE_LOG(LogTemp, Warning, TEXT("Weapon Attached!"));
	}
}

void UWeaponComponent::PlayAnimMontage(UAnimMontage* AnimMontageToPlay)
{
	if (AnimMontageToPlay && AnimInstance)
	{
		AnimInstance->Montage_Play(AnimMontageToPlay, 1.0f);
		//UE_LOG(LogTemp, Warning, TEXT("AnimMontage is Here!! Montage: %s"), *AnimMontageToPlay->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AnimMontage is Null!!"));
	}
}
