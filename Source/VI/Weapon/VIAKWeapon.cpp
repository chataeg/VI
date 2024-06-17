// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VIAKWeapon.h"
#include "Kismet/GameplayStatics.h"
#include "Character/VICharacter.h"
#include "Player/VIPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/PointLightComponent.h"
#include "Kismet/KismetMathLibrary.h"


AVIAKWeapon::AVIAKWeapon()
{

	MaxAmmo = 30;
	AmmoCount = 30;
	ReloadTime = 2.0f;
	BulletSpread = 2000.0f;

	

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> AK_RiggedRef(TEXT("/Script/Engine.SkeletalMesh'/Game/WeaponAssets/RiggedMeshes/AK_rigged.AK_rigged'"));
	if (AK_RiggedRef.Object != nullptr)
	{
		Mesh->SetSkeletalMesh(AK_RiggedRef.Object);
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

	static ConstructorHelpers::FObjectFinder<USoundAttenuation> AKSoundAttenuationRef(TEXT("/Script/Engine.SoundAttenuation'/Game/WeaponAssets/SoundFX/AK_Attenuation.AK_Attenuation'"));
	if (AKSoundAttenuationRef.Object)
	{
		AKSoundAttenuationSettings = AKSoundAttenuationRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USoundConcurrency> AKSoundConcurrencyRef(TEXT("/Script/Engine.SoundConcurrency'/Game/WeaponAssets/SoundFX/AK_Concurrency.AK_Concurrency'"));
	if (AKSoundConcurrencyRef.Object)
	{
		AKSoundConcurrencySettings = AKSoundConcurrencyRef.Object;
	}


}

void AVIAKWeapon::Fire()
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
						AKSoundAttenuationSettings,
						AKSoundConcurrencySettings);

				}

			}

		}
	}
}

void AVIAKWeapon::Reload()
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

void AVIAKWeapon::AmmoCheck()
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
						AKSoundAttenuationSettings,
						AKSoundConcurrencySettings);

					Mesh->PlayAnimation(FireActionAnimation, false);


					/* Start Line Tracing */
					LineTrace();


				}
			}

		}

	}

}

void AVIAKWeapon::LineTrace()
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
					// Muzzle Linetrace
					/*
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
					
					//SpawnDecalTracer(Muzzle->GetComponentLocation(), HitResultMuzzle.ImpactPoint, HitResultMuzzle.ImpactPoint);

					*/
					SpawnDecalTracer(Muzzle->GetComponentLocation(), HitResult.ImpactPoint, HitResult.ImpactPoint);
					
				}
			

			}
			

		}
	}
	
	//return TTuple<FVector, FVector>(FVector::ZeroVector, FVector::ZeroVector);


}

void AVIAKWeapon::SpawnDecalTracer(FVector Location, FVector SpawnTransformLocation, FVector ImpactPoint)
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

void AVIAKWeapon::MuzzleFlash()
{
	PointLight->SetIntensity(20000.0f);
	
	MuzzleFlashMesh->SetVisibility(true);

	FTimerHandle TimerHandle;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, FTimerDelegate::CreateLambda([&](){

		PointLight->SetIntensity(0.0f);

		MuzzleFlashMesh->SetVisibility(false);

	}), 0.1f, false);

	MuzzleFlashMesh->SetRelativeRotation(FRotator(FMath::RandRange(-90.0f,90.0f),90.0f,90.f));

	MuzzleFlashMesh->SetWorldScale3D(FVector(FMath::RandRange(0.2f,0.6f), FMath::RandRange(0.2f, 0.6f), FMath::RandRange(0.2f, 0.6f)));


}
