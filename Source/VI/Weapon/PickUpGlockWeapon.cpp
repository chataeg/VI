// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/PickUpGlockWeapon.h"
#include "Character/VICharacter.h"

// Sets default values
APickUpGlockWeapon::APickUpGlockWeapon()
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

	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &APickUpGlockWeapon::OnOverlapBegin);

}

// Called when the game starts or when spawned
void APickUpGlockWeapon::BeginPlay()
{
	Super::BeginPlay();



	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {

		SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	}), 0.5f, false);
	
}

void APickUpGlockWeapon::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AVICharacter* CastedOtherActor = Cast<AVICharacter>(OtherActor);

	if (CastedOtherActor)
	{
		if (CastedOtherActor->GetSecondaryWeapon()->Class == CastedOtherActor->GetWeaponBaseBpRef())
		{
			CastedOtherActor->GetSecondaryWeapon()->Class = CastedOtherActor->GetGlockWeaponBpRef();
			CastedOtherActor->GetSecondaryWeapon()->AmmoCount = 11;
			CastedOtherActor->GetSecondaryWeapon()->MaxAmmo = 11;
			CastedOtherActor->GetSecondaryWeapon()->ReloadTime = 1.5f;
			CastedOtherActor->GetSecondaryWeapon()->BulletSpread = 1500.0f;

			Destroy();

		}
	}


}
