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

	bUseControllerRotationYaw = true;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(1200.f);
	AdsComponent = CreateDefaultSubobject<UAdsComponent>("Ads Component");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("Weapon Component");
}
void ABPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

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
		{
			UE_LOG(LogTemp, Warning, TEXT("ADS Binded!"));
			EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Triggered, 
				this, &ABPlayerCharacter::HandleAimInputHold);
			EnhancedInputComponent->BindAction(EndAimInputAction, ETriggerEvent::Triggered, 
				this, &ABPlayerCharacter::HandleAimInputReleased);
		}
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
	if (GetVelocity().Length() <= 0)
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
		bUseControllerRotationYaw = false;
	}
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
	InterpCharacterRotation();
	InputValue.Normalize();

	AddMovementInput(GetLookRightDirection() * InputValue.X + GetMoveForwardDirection() * InputValue.Y);
}

void ABPlayerCharacter::InterpCharacterRotation()
{
	if (bUseControllerRotationYaw)
		return;

	GetCharacterMovement()->bOrientRotationToMovement = false; //allowing the interp
	FRotator CurrentRotation = GetActorRotation();

	float TargetYaw = GetController()->GetControlRotation().Yaw;
	FRotator TargetRotation = CurrentRotation;
	TargetRotation.Yaw = TargetYaw;

	FRotator NewRotation = CurrentRotation;
	
	NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, GetWorld()->DeltaTimeSeconds, InterpCharacterRotationSpeed);
	SetActorRotation(NewRotation);

	if (FMath::IsNearlyEqual(CurrentRotation.Yaw, TargetRotation.Yaw, 5.f))
	{
		bUseControllerRotationYaw = true;
	}
}

void ABPlayerCharacter::HandleFireInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent)
		return;

	WeaponComponent->TryFireWeapon(ViewCam, ECC_Target);
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
}
void ABPlayerCharacter::HandleDropInput(const FInputActionValue& InputActionValue)
{
	if (!WeaponComponent || !WeaponComponent->GetCurrentWeaponData())
		return;

	WeaponComponent->TryDropCurrentWeapon();
}

void ABPlayerCharacter::HandleAimInputHold(const FInputActionValue& InputActionValue)
{
	if (AdsComponent)
	{
		AdsComponent->SetIsAimingState(true);
	}
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ABPlayerCharacter::HandleAimInputReleased(const FInputActionValue& InputActionValue)
{
	if (AdsComponent)
	{
		AdsComponent->SetIsAimingState(false);
	}
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
