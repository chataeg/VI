// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VIGlockWeapon.h"

#include "Kismet/GameplayStatics.h"
#include "Character/VICharacter.h"
#include "Player/VIPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/KismetMathLibrary.h"

#include "VI.h"

AVIGlockWeapon::AVIGlockWeapon()
{

	MaxAmmo = 11;
	AmmoCount = 11;
	ReloadTime = 1.5f;
	BulletSpread = 1500.0f;



	static ConstructorHelpers::FObjectFinder<USkeletalMesh> Glock_RiggedRef(TEXT("/Script/Engine.SkeletalMesh'/Game/WeaponAssets/RiggedMeshes/Glock_Rigged.Glock_Rigged'"));
	if (Glock_RiggedRef.Object != nullptr)
	{
		Mesh->SetSkeletalMesh(Glock_RiggedRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> GunShotWavRef(TEXT("/Script/Engine.SoundWave'/Game/WeaponAssets/SoundFX/Gunshot.Gunshot'"));
	if (GunShotWavRef.Object)
	{
		GunShotWav = GunShotWavRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundWave> DryFireWavRef(TEXT("/Script/Engine.SoundWave'/Game/WeaponAssets/SoundFX/DryFire.DryFire'"));
	if (DryFireWavRef.Object)
	{
		DryFireWav = DryFireWavRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> GlockSoundAttenuationRef(TEXT("/Script/Engine.SoundAttenuation'/Game/WeaponAssets/SoundFX/AK_Attenuation.AK_Attenuation'"));
	if (GlockSoundAttenuationRef.Object)
	{
		GlockSoundAttenuationSettings = GlockSoundAttenuationRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundConcurrency> GlockSoundConcurrencyRef(TEXT("/Script/Engine.SoundConcurrency'/Game/WeaponAssets/SoundFX/AK_Concurrency.AK_Concurrency'"));
	if (GlockSoundConcurrencyRef.Object)
	{
		GlockSoundConcurrencySettings = GlockSoundConcurrencyRef.Object;
	}


}

void AVIGlockWeapon::Fire()
{
	if (AmmoCount > 0)
	{
		AmmoCheck();
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Ammo is Zero"));
		if (UWorld* World = GetWorld())
		{
			APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

			if (PC)
			{
				AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());
				if (Character)
				{
					UGameplayStatics::PlaySoundAtLocation(this,
						DryFireWav,
						Character->GetCamera()->GetComponentLocation(),
						FMath::RandRange(0.7f, 1.2f) + 2.5f,
						FMath::RandRange(0.7f, 1.2f),
						0.0f,
						GlockSoundAttenuationSettings,
						GlockSoundConcurrencySettings);

				}

			}

		}
	}

}

void AVIGlockWeapon::Reload()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

		if (PC)
		{
			AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());

			if (Character)
			{
				if (AmmoCount < MaxAmmo)
				{
					if (!bDoOnceReload && !Character->GetbIsReloading())
					{
						Character->SetbIsReloading(true);

						Character->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(GlockReloadActionMontage, 1.0f);

						Mesh->PlayAnimation(ReloadActionAnimation, false);


						FTimerHandle ReloadTimeHandle;

						AVICharacter* CapturedCharacter = Character;

					
						GetWorld()->GetTimerManager().SetTimer(ReloadTimeHandle, FTimerDelegate::CreateLambda([&, CapturedCharacter]()
						{
							CapturedCharacter->SetbIsReloading(false);

							AmmoCount = MaxAmmo;

						//	DF("EndTimer bisRelaoding %d", Character->GetbIsReloading())

								// TimerHandle ÃÊ±âÈ­
								GetWorld()->GetTimerManager().ClearTimer(ReloadTimeHandle);
						}), ReloadTime, false);

						bDoOnceReload = false;
					}
				}
			}
		}
	}
}


void AVIGlockWeapon::AmmoCheck()
{
	//DF("Ammo %d",AmmoCount);

	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

		if (PC)
		{
			AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());
			if (Character)
			{
				if (!Character->GetbIsReloading())
				{
					AmmoCount--;

					/* Play Sound */
					UGameplayStatics::PlaySoundAtLocation(this,
						GunShotWav,
						Character->GetCamera()->GetComponentLocation(),
						FMath::RandRange(0.7f, 1.2f) + 2.5f,
						FMath::RandRange(0.7f, 1.2f),
						0.0f,
						GlockSoundAttenuationSettings,
						GlockSoundConcurrencySettings);

					Mesh->PlayAnimation(FireActionAnimation, false);


					/* Start Line Tracing */
					LineTrace();


				}
			}

		}

	}
}

void AVIGlockWeapon::LineTrace()
{
	if (UWorld* World = GetWorld())
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);

		if (PC)
		{
			AVICharacter* Character = Cast<AVICharacter>(PC->GetCharacter());

			if (Character)
			{

				// Camera Linetrace

				FHitResult HitResult;
				FVector TraceStart = Character->GetCamera()->GetComponentLocation();
				FVector TraceEnd = Character->GetCamera()->GetForwardVector() * 20000.0f + TraceStart;


				/* Bullet Spread */

				if (Character->GetVelocity().Length() > 20.0f)
				{
					FVector BulletSpreadRandVector(
						FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
						, FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
						, FMath::RandRange(BulletSpread * -1.0f, BulletSpread)
					);

					TraceEnd = TraceEnd + BulletSpreadRandVector;

				}


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
					SpawnDecalTracer(Muzzle->GetComponentLocation(), HitResult.ImpactPoint, HitResult.ImpactPoint);


					if (Character->GetbADS())
					{
						Character->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(GlockADSFireActionMontage, 1.0f);
					}
					else
					{
						Character->GetFirstPersonMesh()->GetAnimInstance()->Montage_Play(GlockFireActionMontage, 1.0f);;
					}


				}


			}


		}
	}
}

void AVIGlockWeapon::SpawnDecalTracer(FVector Location, FVector SpawnTransformLocation, FVector ImpactPoint)
{
	if (UWorld* World = GetWorld())
	{
		if (BulletDecalRef != nullptr)
		{
			FTransform TFDecal;
			TFDecal.SetLocation(SpawnTransformLocation);

			AActor* Decal = World->SpawnActor<AActor>(BulletDecalRef, TFDecal);
			Decal->SetActorScale3D(FVector(0.025f, 0.025f, 0.025f));
			Decal->SetActorRotation(UKismetMathLibrary::MakeRotFromX(ImpactPoint).Quaternion());

			if (FMath::RandRange(1, 10) >= 0)
			{
				FTransform TF;
				TF.SetLocation(Location);

				FVector Direction = ImpactPoint - Location;
				Direction.Normalize();
				FRotator TracerRotation = Direction.Rotation();


				TF.SetRotation(TracerRotation.Quaternion());
				TF.SetScale3D(FVector(0.05f, 0.05f, 0.05f));

				World->SpawnActor<AActor>(TracerRoundRef, TF);

			}

			MuzzleFlash();

			return;
		}
	}

}

void AVIGlockWeapon::MuzzleFlash()
{
	PointLight->SetIntensity(20000.0f);

	MuzzleFlashMesh->SetVisibility(true);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&]() {

		PointLight->SetIntensity(0.0f);

		MuzzleFlashMesh->SetVisibility(false);

	}), 0.1f, false);

	MuzzleFlashMesh->SetRelativeRotation(FRotator(FMath::RandRange(-90.0f, 90.0f), 90.0f, 90.f));

	MuzzleFlashMesh->SetWorldScale3D(FVector(FMath::RandRange(0.1f, 0.3f), FMath::RandRange(0.1f, 0.3f), FMath::RandRange(0.1f, 0.3f)));

}

void AVIGlockWeapon::UnEquip()
{
	FGunData* GunDataPtr;	
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AVICharacter* CharacterPtr = Cast<AVICharacter>(PC->GetCharacter());
	GunDataPtr = CharacterPtr->GetSecondaryWeapon();

	GunDataPtr->Class = CharacterPtr->GetGlockWeaponBpRef();
	GunDataPtr->AmmoCount = AmmoCount;
	GunDataPtr->MaxAmmo = MaxAmmo;
	GunDataPtr->ReloadTime = ReloadTime;
	GunDataPtr->BulletSpread = BulletSpread;


}

void AVIGlockWeapon::Equip()
{
}
	