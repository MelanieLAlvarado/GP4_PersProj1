// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerCharacter.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Camera/CameraComponent.h" 
#include "Components/AdsComponent.h"
#include "Weapon/WeaponComponent.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup/Pickup.h"
#include "Weapon/WeaponDataAsset.h"
#include "Widget/DamageIndicatorWidget.h"
#include "Widget/GameplayWidget.h"
#include "Widget/WeaponInfoWidget.h"

#define ECC_Target ECC_GameTraceChannel1

ABPlayerCharacter::ABPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = 500.f;
	CameraBoom->SetRelativeLocation(FVector(0, CamBoomYOffset, 0));

	bUseControllerRotationYaw = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1200.f);
	AdsComponent = CreateDefaultSubobject<UAdsComponent>("ADS Component");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Weapon Component");
	//OnWeaponUpdated.AddUObject(this, &ABPlayerCharacter::WeponUpdated);
	//OnWeaponUpdatedDynamic.AddDynamic(this, &ABPlayerCharacter::WeaponUpdatedDynamic);
}//Target Arm Length = 500 | Socket Offset = FVector(0, 75, 0) 

void ABPlayerCharacter::PawnClientRestart()
{
	Super::PawnClientRestart();
	APlayerController* OwningPlayerController = GetController<APlayerController>();
	if (OwningPlayerController)
	{
		UEnhancedInputLocalPlayerSubsystem* EnhancedSubsystem =
			OwningPlayerController->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		EnhancedSubsystem->RemoveMappingContext(GameplayInputMappingContext);
		EnhancedSubsystem->AddMappingContext(GameplayInputMappingContext, 0);
	}
}

// Called to bind functionality to input
void ABPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent)
	{
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleLookInput);
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleMoveInput);
		EnhancedInputComponent->BindAction(FireInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleFireInput);
		EnhancedInputComponent->BindAction(InteractInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleInteractInput);
		EnhancedInputComponent->BindAction(ReloadInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleReloadInput);

		EnhancedInputComponent->BindAction(DropCurrentInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleDropInput);

		if (AdsComponent)
			EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Triggered, 
				this, &ABPlayerCharacter::HandleAimInputHold);
			EnhancedInputComponent->BindAction(EndAimInputAction, ETriggerEvent::Triggered, 
				this, &ABPlayerCharacter::HandleAimInputReleased);
	}
}

void ABPlayerCharacter::Tick(float DeltaTime)
{

}

void ABPlayerCharacter::InitializeWeaponComponentUI(UGameplayWidget* GameplayWidget)
{
	if (!WeaponComponent)
		return;

	UWeaponInfoWidget* WeaponWidget = GameplayWidget->GetWeaponInfoWidget();
	if (!WeaponWidget)
		return;

	WeaponComponent->SetWeaponInfoWidget(WeaponWidget);
}

UCameraComponent* ABPlayerCharacter::GetViewCamera()
{
	return ViewCam;
}

USpringArmComponent* ABPlayerCharacter::GetCameraBoom()
{
	return CameraBoom;
}

void ABPlayerCharacter::SetInteractable(AActor* InteractableToSet)
{
	Interactable = InteractableToSet;
}

void ABPlayerCharacter::HandleLookInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	AddControllerYawInput(InputValue.X);
	AddControllerPitchInput(-InputValue.Y);
}

void ABPlayerCharacter::HandleMoveInput(const FInputActionValue& InputActionValue)
{
	FVector2D InputValue = InputActionValue.Get<FVector2D>();
	if (InputValue.Length() < 0)
	{
		return;
	}
	InputValue.Normalize();

	AddMovementInput(GetLookRightDirection() * InputValue.X + GetMoveForwardDirection() * InputValue.Y);
}

void ABPlayerCharacter::HandleFireInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent)
		return;

	FHitResult HitResult;
	if (!WeaponComponent->TryFireWeapon(HitResult, ViewCam, ECC_Target))
		return;
	UE_LOG(LogTemp, Warning, TEXT("TryFire HitActor: %s"), *HitResult.ToString());
	//send values into handle fire input



	//AddControllerPitchInput(-1.0f);//recoil test

	/*APlayerController* PlayerController = GetController<APlayerController>();
	if (DamageWidgetClass != NULL && PlayerController != NULL)
	{ 
		UDamageIndicatorWidget* DamageWidget = CreateWidget<UDamageIndicatorWidget>(PlayerController, DamageWidgetClass);

		FVector2D ScreenLocation;

		float ScreenSize = UWidgetLayoutLibrary::GetViewportScale(GetWorld());
		PlayerController->ProjectWorldLocationToScreen(HitResult.ImpactPoint, ScreenLocation, false);
		//DamageWidget->SetPositionInViewport(ScreenLocation);

		DamageWidget->AddToViewport();
		DamageWidget->SetRenderTranslation(ScreenLocation * FMath::Pow(ScreenSize, -1.0));
	}*/
}

void ABPlayerCharacter::HandleInteractInput(const FInputActionValue& InputActionValue)
{
	if (!Interactable)
		return;

	IInteractInterface* InteractInterface = Cast<IInteractInterface>(Interactable);
	if (InteractInterface)
	{
		InteractInterface->Interact(this);
	}

	//OnWeaponUpdated.Broadcast(1,2);
	//OnWeaponUpdatedDynamic.Broadcast(1,2);
}
void ABPlayerCharacter::HandleDropInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent || !WeaponComponent->GetCurrentWeaponData())
		return;

	WeaponComponent->TryDropCurrentWeapon();
}

void ABPlayerCharacter::HandleAimInputHold(const FInputActionValue& InputActionValue)
{
	AdsComponent->SetIsAimingState(true);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ABPlayerCharacter::HandleAimInputReleased(const FInputActionValue& InputActionValue)
{
	AdsComponent->SetIsAimingState(false);
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void ABPlayerCharacter::HandleReloadInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent)
		return;
	
	WeaponComponent->ReloadWeapon();
}

FVector ABPlayerCharacter::GetLookRightDirection() const
{
	return ViewCam->GetRightVector();
}

FVector ABPlayerCharacter::GetLookForwardDirection() const
{
	return ViewCam->GetForwardVector();
}

FVector ABPlayerCharacter::GetMoveForwardDirection() const
{
	return FVector::CrossProduct(GetLookRightDirection(), FVector::UpVector);
}

bool ABPlayerCharacter::TryCanPickup(class APickup* Pickup)
{
	AWeaponPickup* WeaponPickup = Cast<AWeaponPickup>(Pickup);
	if (WeaponPickup && WeaponComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon found!"));
		WeaponComponent->SetCurrentWeapon(WeaponPickup);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Cannot Pickup"));
	return false;
}
