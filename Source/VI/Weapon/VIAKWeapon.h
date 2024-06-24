// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/VIWeaponbase.h"
#include "VIAKWeapon.generated.h"

/* 무기 AK47 클래스 */
UCLASS()
class VI_API AVIAKWeapon : public AVIWeaponbase
{
	GENERATED_BODY()
	
public:

	AVIAKWeapon();

	
	virtual void Fire() override;
	
	virtual void Reload() override;
	
	virtual void AmmoCheck() override;
	
	virtual void LineTrace() override;
	
	virtual void SpawnDecalTracer(FVector Location, FVector SpawnTransformLocation, FVector  ImpactPoint) override;
	
	virtual void MuzzleFlash() override;

	virtual void UnEquip() override;


	// Asset Section

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimationAsset> FireActionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation)
	TObjectPtr<class UAnimationAsset> ReloadActionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr <class USoundWave> GunShotWav;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr <class USoundWave> DryFireWav;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr <class USoundAttenuation> AKSoundAttenuationSettings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sound)
	TObjectPtr <class USoundConcurrency> AKSoundConcurrencySettings;


	UPROPERTY(EditAnywhere,BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	FVector AimOffset;




	// 블루프린트에서 에셋 지정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKFireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKReloadActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKADSFireActionMontage;








};
