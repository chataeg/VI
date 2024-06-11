// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VIWeaponbase.h"
#include "Kismet/GameplayStatics.h"
#include "Character/VICharacter.h"
#include "Player/VIPlayerController.h"
#include "Camera/CameraComponent.h"
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

	MaxAmmo = 30;
	AmmoCount = 30;
	ReloadTime = 2.0f;
	BulletSpread = 500.0f;

	


	//UE_LOG(LogTemp, Log, TEXT("Weaponbase Constructor"));
}

// Called when the game starts or when spawned
void AVIWeaponbase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AVIWeaponbase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

