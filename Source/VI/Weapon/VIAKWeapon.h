// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/VIWeaponbase.h"
#include "VIAKWeapon.generated.h"

/**
 * 
 */
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

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimationAsset> FireActionAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimationAsset> ReloadActionAnimation;

	UPROPERTY(EditAnywhere)
	TObjectPtr <class USoundWave> GunShotWav;

	UPROPERTY(EditAnywhere)
	TObjectPtr <class USoundWave> DryFireWav;

	UPROPERTY(EditAnywhere)
	TObjectPtr <class USoundAttenuation> AKSoundAttenuationSettings;

	UPROPERTY(EditAnywhere)
	TObjectPtr <class USoundConcurrency> AKSoundConcurrencySettings;









};
