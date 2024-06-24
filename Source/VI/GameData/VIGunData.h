// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon/VIWeaponbase.h"
#include "VIGunData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FGunData{

	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<class AVIWeaponbase> Class;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AmmoCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float  ReloadTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BulletSpread;


};


class VI_API VIGunData
{


public:
	VIGunData();






};
