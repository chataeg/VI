// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VIWeaponbase.h"

// Sets default values
AVIWeaponbase::AVIWeaponbase()
{
 	
	PrimaryWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("PrimaryWeapon"));
	
	PrimaryWeapon->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AK_RiggedRef(TEXT("/Script/Engine.SkeletalMesh'/Game/WeaponAssets/RiggedMeshes/AK_rigged.AK_rigged'"));
	if (AK_RiggedRef.Object != nullptr)
	{
		PrimaryWeapon->SetSkeletalMesh(AK_RiggedRef.Object);
	}

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	
	Muzzle->SetupAttachment(PrimaryWeapon);
	Muzzle->SetRelativeLocation(FVector(62.f, -0.98f, 10.13f));





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

void AVIWeaponbase::Fire()
{
}

void AVIWeaponbase::Reload()
{
}

