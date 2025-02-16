// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/BPlayerCharacter.h"
#include "Camera/CameraComponent.h" 
#include "Components/AdsComponent.h"
#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Framework/InteractInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Pickup/Pickup.h"
#include "Target/Target.h"
#include "Weapon/WeaponDataAsset.h"

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

	if (!WeaponComponent->TryFireWeapon())
		return;

	//send values into handle fire input
	float FireDistance = 1000.0f;

	FHitResult HitResult;
	if (!CalculateFireResult(HitResult))
	{
		return;
	}	
	UE_LOG(LogTemp, Warning, TEXT("Fired Successfully"));
	//process target
	AActor* HitActor = HitResult.GetActor();
	if (!HitActor)
		return;
	
	ATarget* Target = Cast<ATarget>(HitActor);
	if (!Target)
		return;

	HitActor->Destroy();
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

	WeaponComponent->TryDropCurrentWeapon(DropSpawnDistance);

	/*FActorSpawnParameters ActorSpawnParams;
	
	ActorSpawnParams.SpawnCollisionHandlingOverride;

	const FVector SpawnLocation = this->GetActorLocation() + (this->GetActorForwardVector() * DropSpawnDistance);

	APickup* PickupActor = GetWorld()->SpawnActor<APickup>(CurrentWeapon->GetClass(), SpawnLocation, FRotator::ZeroRotator);

	if (PickupActor != NULL)
	{
		PickupActor->InitializeWithDataAsset();
		CurrentWeapon = NULL;
	}*/
}

void ABPlayerCharacter::HandleAimInputHold(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming hold"));
	AdsComponent->SetIsAimingState(true);
	GetCharacterMovement()->bOrientRotationToMovement = false;
	bUseControllerRotationYaw = true;
}

void ABPlayerCharacter::HandleAimInputReleased(const FInputActionValue& InputActionValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Aiming release"));
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
	
	//DropCurrent Weapon?
	
	if (WeaponPickup && WeaponComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Weapon found!"));
		WeaponComponent->SetCurrentWeapon(WeaponPickup);
		//WeaponPickupArray.Add(WeaponPickup);
		return true;
	}
	UE_LOG(LogTemp, Warning, TEXT("Cannot Pickup"));
	return false;
}

bool ABPlayerCharacter::CalculateFireResult(FHitResult HitResult)
{
	float FireDistance = 1000.0f;

	const FVector CamLineStart = ViewCam->GetComponentLocation();
	const FVector CamLineEnd = CamLineStart + ViewCam->GetForwardVector() * FireDistance;
	if (!GetWorld()->LineTraceSingleByChannel(HitResult, CamLineStart, CamLineEnd, ECC_Target))
	{
		return false;
	}
	DrawDebugLine(GetWorld(), CamLineStart, CamLineEnd, FColor::Blue, true, 3.f);

	const FVector PlayerLineStart = GetActorLocation();
	const FVector PlayerLineEnd = HitResult.ImpactPoint;

	if (!GetWorld()->LineTraceSingleByChannel(HitResult, PlayerLineStart, PlayerLineEnd, ECC_Target))
	{
		return false;
	}
	DrawDebugLine(GetWorld(), PlayerLineEnd, PlayerLineStart, FColor::Red, true, 3.f);
	return true;
}