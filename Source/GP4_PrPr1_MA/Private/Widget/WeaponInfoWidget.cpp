// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WeaponInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UWeaponInfoWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWeaponInfoWidget::WeaponUpdated(UWeaponDataAsset* WeaponData, int CurrentAmmoValue)
{
	if (WeaponData == NULL)
	{
		Icon->SetBrushFromTexture(NULL);
		ResetValue();
		return;
	}

	if (WeaponData->GetWeaponIcon())
	{
		UTexture2D* WeaponIconTexture = WeaponData->GetWeaponIcon();
		Icon->SetBrushFromTexture(WeaponIconTexture);
	}
	SetValue(CurrentAmmoValue, WeaponData->GetMaxAmmo());
}

void UWeaponInfoWidget::ResetValue()
{
	ValueText->SetText(FText::FromString(""));
}

void UWeaponInfoWidget::SetValue(int NewValue, int NewMaxValue)
{
	if (NewMaxValue == 0)
		return;
	FNumberFormattingOptions FormatingOptions = FNumberFormattingOptions().SetMaximumFractionalDigits(0);
	ValueText->SetText(
		FText::Format(
			FTextFormat::FromString("{0}/{1}"),
			FText::AsNumber(NewValue, &FormatingOptions),
			FText::AsNumber(NewMaxValue, &FormatingOptions))
	);
}
