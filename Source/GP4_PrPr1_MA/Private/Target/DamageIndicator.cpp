// Fill out your copyright notice in the Description page of Project Settings.


#include "Target/DamageIndicator.h"
#include "Widget/DamageIndicatorWidget.h"

// Sets default values
ADamageIndicator::ADamageIndicator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget Component");
	WidgetComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ADamageIndicator::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Damage Indicator (Actor) is in scene!"));
}

// Called every frame
void ADamageIndicator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorLocation(FVector(GetActorLocation().X, GetActorLocation().Y, GetActorLocation().Z + MoveUpSpeed));
}

void ADamageIndicator::InitializeDamageIndicator(float DamageToDisplay)
{
	if (!DamageIndicatorWidget )//&& !InstigatorController)//Do we need the controllwe?
	{
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Damage Indicator (Actor) is initialized!"));

	WidgetComponent->SetWidget(DamageIndicatorWidget);
	if (UDamageIndicatorWidget* DamageWidget = Cast<UDamageIndicatorWidget>(DamageIndicatorWidget))
	{
		DamageWidget->DisplayDamage(DamageToDisplay);
	}

	WidgetComponent->SetHiddenInGame(false);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &ADamageIndicator::TimerEnd, TimerDuration, false);

}

void ADamageIndicator::TimerEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	//RemoveFromViewport();
	Destroy();
}


