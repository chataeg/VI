// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VICharacterBase.h"

// Sets default values
AVICharacterBase::AVICharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AVICharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AVICharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

