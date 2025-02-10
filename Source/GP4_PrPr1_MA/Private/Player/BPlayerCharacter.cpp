// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/Weapon.h"

ABPlayerCharacter::ABPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->bUsePawnControlRotation = true;
	CameraBoom->TargetArmLength = CameraBoomDefaultLength;
	CameraBoom->SetRelativeLocation(FVector(0, CamBoomYOffset, 0));

	bUseControllerRotationYaw = false;

	ViewCam = CreateDefaultSubobject<UCameraComponent>("View Cam");
	ViewCam->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	ViewCam->SetFieldOfView(DefaultFOV);

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(1200.f);
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
		EnhancedInputComponent->BindAction(AimInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleAimInputHold);
		EnhancedInputComponent->BindAction(EndAimInputAction, ETriggerEvent::Triggered, this, &ABPlayerCharacter::HandleAimInputReleased);
	}
}

void ABPlayerCharacter::Tick(float DeltaTime)
{
	if (!bIsViewLerp)
	{
		return;
	}
	LerpAlpha += DeltaTime;

	if (bIsAiming)
	{
		LerpCameraBoomLength(CameraBoomDefaultLength, CameraBoomAimLength);
		LerpFOV(DefaultFOV, AimFOV);
		bIsViewLerp = !(CurrentFOV <= AimFOV && CameraBoomCurrentLength <= CameraBoomAimLength);
	}
	else
	{
		LerpCameraBoomLength(CameraBoomAimLength, CameraBoomDefaultLength);
		LerpFOV(AimFOV, DefaultFOV);
		bIsViewLerp = !(CurrentFOV >= DefaultFOV && CameraBoomCurrentLength >= CameraBoomDefaultLength);
	}
	ViewCam->SetFieldOfView(CurrentFOV);

	if (!bIsViewLerp)
	{
		LerpAlpha = 0.f;
	}
	UE_LOG(LogTemp, Warning,TEXT("Fov: %f"), CurrentFOV);
}

void ABPlayerCharacter::SetInteractable(AActor* InteractableToSet)
{
	Interactable = InteractableToSet;
}

void ABPlayerCharacter::LerpCameraBoomLength(float StartValue, float EndValue)
{
	CameraBoomCurrentLength = FMath::Lerp(StartValue, EndValue, LerpAlpha);
}

void ABPlayerCharacter::LerpFOV(float StartValue, float EndValue)
{
	CurrentFOV = FMath::Lerp(StartValue, EndValue, LerpAlpha);
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
	if (!CurrentWeapon)
		return;

	//get current weapon
	//send values into handle fire input

	float FireDistance = 20.0f;

	const FVector LineStart = ViewCam->GetComponentLocation();
	const FVector LineEnd = LineStart + ViewCam->GetForwardVector() * FireDistance;

	FHitResult HitResult;
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, LineStart, LineEnd, ECC_WorldStatic))
	{
		return;
	}

	AActor* HitActor = HitResult.GetActor();
	//process target
	/*
	if (ATarget* Target = Cast<ATarget>(HitActor))
	{
		//process target damage and such
	}
	*/
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

void ABPlayerCharacter::HandleAimInputHold(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming hold"));

	bIsAiming = true;
	bIsViewLerp = true;
}

void ABPlayerCharacter::HandleAimInputReleased(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming release"));
	bIsAiming = false;
	bIsViewLerp = true;
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

bool ABPlayerCharacter::TryCanPickup(TSubclassOf<class AActor> PickupClass)
{
	/*TSubclassOf<class AWeapon>* PickupWeaponClass = Cast<TSubclassOf<class AWeapon>>(PickupClass);
	if (!PickupWeaponClass)
		return false;

	if (CurrentWeapon->IsA(PickupWeaponClass))
		return false;*///already has this as current weapon
	
	//CurrentWeapon = 

	return true;
}
