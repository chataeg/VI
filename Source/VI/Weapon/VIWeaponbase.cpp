// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VIWeaponbase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/VICharacter.h"
#include "Player/VIPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "VI.h"
#include "DrawDebugHelpers.h"


// Sets default values
AVIWeaponbase::AVIWeaponbase()
{
 	
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PrimaryWeapon"));
	Mesh->SetupAttachment(RootComponent);

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	Muzzle->SetupAttachment(Mesh);
	Muzzle->SetRelativeLocation(FVector(62.f, -0.98f, 10.13f));


	PointLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PointLight"));
	PointLight->SetupAttachment(Mesh);
	PointLight->SetRelativeLocation(FVector(59.f,-8.5f,11.2f));
	PointLight->Intensity = 0.0f;

	MuzzleFlashMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MuzzleFlashMesh"));
	MuzzleFlashMesh->SetupAttachment(Mesh);
	MuzzleFlashMesh->SetRelativeRotation(FRotator(-90.f,0.0f,0.0f));
	MuzzleFlashMesh->SetRelativeScale3D(FVector(0.6f, 0.6f, 0.6f));
	MuzzleFlashMesh->SetRelativeLocation(FVector(74.9f, 0.0f, 9.75f));
	

	MaxAmmo = 30;
	AmmoCount = 30;
	ReloadTime = 2.0f;
	BulletSpread = 2000.0f;

	static ConstructorHelpers::FClassFinder<AActor> BulletDecalBpRef(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_BulletDecal.BP_BulletDecal_C'"));
	if (BulletDecalBpRef.Class)
	{
		BulletDecalRef = BulletDecalBpRef.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> TracerRoundBpRef(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_TracerRound.BP_TracerRound_C'"));
	if (TracerRoundBpRef.Class)
	{
		TracerRoundRef = TracerRoundBpRef.Class;
	}


}

