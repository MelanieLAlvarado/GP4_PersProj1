// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/BCharacter.h"
#include "BCharacter.h"

// Sets default values
ABCharacter::ABCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ABCharacter::ServerSideInit()
{

}

void ABCharacter::ClientSideInit()
{
}




