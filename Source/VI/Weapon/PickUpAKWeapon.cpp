// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PickUpAKWeapon.h"
#include "Character/VICharacter.h"
#include "VI.h"

APickUpAKWeapon::APickUpAKWeapon()
{

	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetupAttachment(RootComponent);

	StaticMesh->SetGenerateOverlapEvents(false);

	StaticMesh->SetCollisionProfileName(TEXT("PickUpWeapon"));

	StaticMesh->SetSimulatePhysics(true);

	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));

	SphereCollision->SetupAttachment(StaticMesh);

	SphereCollision->SetSphereRadius(65.0f);

	SphereCollision->SetRelativeLocation(FVector(18.6f, 0.0f, 10.2f));

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickUpAKWeapon::OnOverlapBegin);

	

}

void APickUpAKWeapon::BeginPlay()
{
	Super::BeginPlay();


	
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&](){

		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}), 0.5f, false);


}

void APickUpAKWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
	AVICharacter* CastedOtherActor = Cast<AVICharacter>(OtherActor);

	if (CastedOtherActor)
	{
		D("Casted");
		if (CastedOtherActor->GetPrimaryWeapon()->Class == CastedOtherActor->GetWeaponBaseBpRef())
		{
			CastedOtherActor->GetPrimaryWeapon()->Class = CastedOtherActor->GetAKWeaponBpRef();
			CastedOtherActor->GetPrimaryWeapon()->AmmoCount = 30;
			CastedOtherActor->GetPrimaryWeapon()->MaxAmmo = 30;
			CastedOtherActor->GetPrimaryWeapon()->ReloadTime = 2.0f;
			CastedOtherActor->GetPrimaryWeapon()->BulletSpread = 2000.0f;

			Destroy();

		}
	}


}
