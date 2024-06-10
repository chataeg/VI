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

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AK_RiggedRef(TEXT("/Script/Engine.SkeletalMesh'/Game/WeaponAssets/RiggedMeshes/AK_rigged.AK_rigged'"));
	if (AK_RiggedRef.Object != nullptr)
	{
		Mesh->SetSkeletalMesh(AK_RiggedRef.Object);
	}

	Muzzle = CreateDefaultSubobject<USceneComponent>(TEXT("Muzzle"));
	
	Muzzle->SetupAttachment(Mesh);
	Muzzle->SetRelativeLocation(FVector(62.f, -0.98f, 10.13f));

	MaxAmmo = 30;
	AmmoCount = 30;
	ReloadTime = 2.0f;
	BulletSpread = 500.0f;


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
	if (AmmoCount > 0)
	{
		AmmoCheck();
	}
}

void AVIWeaponbase::Reload()
{
	Mesh->PlayAnimation(ReloadActionAnimation, false);

	FTimerHandle ReloadTimeHandle;


	GetWorld()->GetTimerManager().SetTimer(ReloadTimeHandle, FTimerDelegate::CreateLambda([&]()
	{
		SetAmmoCount(GetMaxAmmo());
		
		// TimerHandle ÃÊ±âÈ­
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimeHandle);
	}), ReloadTime, false);
}

void AVIWeaponbase::AmmoCheck()
{
	
	AmmoCount--;

	//DF("Ammo %d",AmmoCount);

	if (UWorld* World = GetWorld())
	{
		// Get the player controller for the first player (index 0)
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
		
		if (PC)
		{
			AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());

			if (!Character->GetbIsReloading())
			{
				Mesh->PlayAnimation(FireActionAnimation, false);
				UE_LOG(LogTemp, Log, TEXT("Fire Animation "));
				LineTrace();
			}

		
		}
	
	}

}

void AVIWeaponbase::LineTrace()
{
	if (UWorld* World = GetWorld())
	{
		// Get the player controller for the first player (index 0)
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

		if (PC)
		{
			AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());

			FHitResult HitResult;
			 FVector TraceStart = Character->GetCamera()->GetComponentLocation();
			 FVector TraceEnd = Character->GetCamera()->GetForwardVector() * 20000.0f + TraceStart;
			

			/* ÅºÆÛÁü ¼³Á¤
			FVector BulletSpreadRandVector(
			FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
			, FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
			, FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
			);
				
			TraceEnd = TraceEnd + BulletSpreadRandVector;
			*/

			TArray<AActor*> ActorsToIgnore;
			FColor ColorBeforeHit = FColor::Red;
			FColor ColorAfterHit = FColor::Green;
			float ColorsLifeTime = 5.f;

			if (UKismetSystemLibrary::LineTraceSingle(
				World,
				TraceStart,
				TraceEnd,
				UEngineTypes::ConvertToTraceType(ECC_Visibility),
				false,
				ActorsToIgnore,
				EDrawDebugTrace::ForDuration,
				HitResult,
				true,
				ColorBeforeHit,
				ColorAfterHit,
				ColorsLifeTime
			))
			{
				FHitResult HitResultMuzzle;
				FVector TraceStartMuzzle = Muzzle->GetComponentLocation();
				FVector TraceEndMuzzle = HitResult.ImpactPoint;

				TArray<AActor*> ActorsToIgnoreMuzzle;
				FColor ColorBeforeHitMuzzle = FColor::Green;
				FColor ColorAfterHitMuzzle = FColor::Green;
				float ColorsLifeTimeMuzzle = 5.f;

				UKismetSystemLibrary::LineTraceSingle(
					World,
					TraceStartMuzzle,
					TraceEndMuzzle,
					UEngineTypes::ConvertToTraceType(ECC_Visibility),
					false,
					ActorsToIgnoreMuzzle,
					EDrawDebugTrace::ForDuration,
					HitResultMuzzle,
					true,
					ColorBeforeHitMuzzle,
					ColorAfterHitMuzzle,
					ColorsLifeTimeMuzzle);


			}

		}
	}



}

