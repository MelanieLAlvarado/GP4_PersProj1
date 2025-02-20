// Fill out your copyright notice in the Description page of Project Settings.


#include "Target/Target.h"
//#include "Components/WidgetComponent.h"
#include "Target/DamageIndicator.h"

// Sets default values
ATarget::ATarget()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATarget::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATarget::HitAction(UWeaponDataAsset* WeaponHitData, FHitResult HitResult)
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

