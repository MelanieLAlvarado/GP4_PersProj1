// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup/Pickup.h"
#include "Player/BPlayerCharacter.h"
#include "Player/BPlayerController.h"
#include "Components/SphereComponent.h"
#include "Weapon/WeaponDataAsset.h"

// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>("Pickup Mesh");
	RootComponent = PickupMesh;

	PickupMesh->SetSimulatePhysics(true);

	SphereCollider = CreateDefaultSubobject<USphereComponent>("Sphere Collider");
	SphereCollider->SetupAttachment(RootComponent);

	OnActorBeginOverlap.AddDynamic(this, &APickup::OnOverlapBegin);
	OnActorEndOverlap.AddDynamic(this, &APickup::OnOverlapEnd);
}

void APickup::InitializeWithDataAsset()
{
	if (!PickupItemClass)
		return;

}

void APickup::Interact(AActor* InteractingActor)
{
	//interact action here.//will change to press button input
	OnPickupCollected(InteractingActor);
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
	SetPickupActive(true);

	if (SphereCollider)
	{
		SphereCollider->SetSimulatePhysics(false);
		SphereCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		SphereCollider->SetCollisionResponseToAllChannels(ECR_Ignore);
		SphereCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	}
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
	//UE_LOG(LogTemp, Warning, TEXT("Overlap started!"));
	ABPlayerCharacter* OverlappedCharacter = Cast<ABPlayerCharacter>(OtherActor);
	if (OverlappedCharacter && IsPickupActive())
	{
		OverlappedCharacter->SetInteractable(this);
	}
}

void APickup::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
	ABPlayerCharacter* OverlappedCharacter = Cast<ABPlayerCharacter>(OtherActor);
	if (OverlappedCharacter && IsPickupActive())
	{
		OverlappedCharacter->SetInteractable(NULL);
	}
}

void APickup::OnPickupCollected(AActor* InteractingActor)
{//(Player pressed "interact" on default Pickup)
	if (!IsPickupActive())
		return;
	if (!InteractingActor)
		return;
	
	ABPlayerCharacter* InteractPlayerCharacter = Cast<ABPlayerCharacter>(InteractingActor);
	if (!InteractPlayerCharacter)
		return;

	if (InteractPlayerCharacter->TryCanPickup(this))
	{
		SetPickupActive(false);
		Destroy();
	}
}

