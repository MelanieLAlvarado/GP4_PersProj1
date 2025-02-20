// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TargetComponent.h"
#include "Target/DamageIndicator.h"

//#define ECC_Target ECC_GameTraceChannel1;



UTargetComponent::UTargetComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	//GetOwner()->CreateDefaultSubobject<UStaticMeshComponent>("Static Mesh Component");

}


// Called when the game starts
void UTargetComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	if (UStaticMeshComponent* TargetMesh = Cast<UStaticMeshComponent>(
		GetOwner()->GetComponentByClass(UStaticMeshComponent::StaticClass())))
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Mesh Found!"));
		TargetMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1,
			ECollisionResponse::ECR_Block);
	}
}


// Called every frame
void UTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
void UTargetComponent::HitAction(UWeaponDataAsset* WeaponHitData, FHitResult HitResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Hit Action (Target)"));
	/*UWidgetComponent* DamageIndicatorComponent = NewObject<UWidgetComponent>(this);

	DamageIndicatorComponent->SetWidget(DamageIndicatorWidget);*/
	if (DamageIndicatorClass && WeaponHitData)
	{
		ADamageIndicator* DamageIndicator = GetWorld()->SpawnActor<ADamageIndicator>(DamageIndicatorClass,
			HitResult.ImpactPoint, FRotator::ZeroRotator);
		DamageIndicator->InitializeDamageIndicator(WeaponHitData->GetDamageToDeal());
	}
}
