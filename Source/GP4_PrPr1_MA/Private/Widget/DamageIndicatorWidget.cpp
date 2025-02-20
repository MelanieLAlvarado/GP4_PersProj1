// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DamageIndicatorWidget.h"
#include "Components/TextBlock.h"
#include "CoreMinimal.h"


void UDamageIndicatorWidget::DisplayDamage(float DamageDealt)
{
	if (!ValueText)
		return;


	float RoundedDamage = FMath::RoundToInt(DamageDealt);
	FString DisplayDamage = FString::FromInt(RoundedDamage);

	if (DamageDealt > RoundedDamage)
	{
		int DecimalDamage = (DamageDealt - RoundedDamage) * 10;
		DisplayDamage = FString::FromInt(RoundedDamage) + "." + FString::FromInt(DecimalDamage);
	}
	ValueText->SetText(FText::FromString(DisplayDamage));

}

