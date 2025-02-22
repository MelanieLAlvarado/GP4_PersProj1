// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AN_EnableCanFire.h"
#include "Weapon/WeaponComponent.h"

void UAN_EnableCanFire::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	UWeaponComponent* WeaponComp = Cast<UWeaponComponent>(MeshComp->GetOwner()->GetComponentByClass<UWeaponComponent>());
	if (WeaponComp)
	{
		WeaponComp->ProcessCanFire();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("WEAPON COMP not FOUND..."));
	}
}
