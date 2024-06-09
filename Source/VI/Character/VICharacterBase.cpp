// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VICharacterBase.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AVICharacterBase::AVICharacterBase()
{

	// Pawn
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	
    GetCapsuleComponent()->InitCapsuleSize(42.0f,96.0f);

	
	/*
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}


	// Set Mesh Location and Rotation
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f)); 
	*/


}

