// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/WeaponInfoWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void UWeaponInfoWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UWeaponInfoWidget::InitializeWithCurrentWeapon(UWeaponDataAsset* WeaponData)
{
	if (!WeaponData)
		return;

	bool bFound = false;
	int NewValue = WeaponData->GetCurrentAmmo();
	int NewMaxValue = WeaponData->GetMaxAmmo();

	if (bFound)
	{
		SetValue(NewValue, NewMaxValue);
	}

}

void UWeaponInfoWidget::UpdateWithCurrentWeapon(UWeaponDataAsset* WeaponData)
{

}

void UWeaponInfoWidget::SetValue(int NewValue, int NewMaxValue)
{
	CachedValue = NewValue;
	CachedMaxValue = NewMaxValue;
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

void UWeaponInfoWidget::ValueChanged(UWeaponDataAsset* WeaponData)
{
	SetValue(WeaponData->GetCurrentAmmo(), CachedMaxValue);
}

void UWeaponInfoWidget::MaxValueChanged(UWeaponDataAsset* WeaponData)
{
	SetValue(CachedValue, WeaponData->GetMaxAmmo());
}
