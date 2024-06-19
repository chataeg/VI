// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimationAsset.h"
#include "VIWeaponbase.generated.h"


/* ������� �⺻ ���밡 �Ǵ� �߻� ���̽� Ŭ���� */
UCLASS(Abstract)
class VI_API AVIWeaponbase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVIWeaponbase();

public:	

	UFUNCTION()
	virtual void Fire() PURE_VIRTUAL(AVIWeaponbase::Fire,);

	UFUNCTION()
	virtual void Reload() PURE_VIRTUAL(AVIWeaponbase::Reload, );

	FORCEINLINE int32 GetAmmoCount() const { return AmmoCount; };
	FORCEINLINE void SetAmmoCount(int32 NewAmmoCount) { AmmoCount = NewAmmoCount; };

	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; };
	FORCEINLINE void SetMaxAmmo(int32 NewMaxAmmo) { MaxAmmo = NewMaxAmmo; };
	
	FORCEINLINE int32 GetReloadTime() const { return ReloadTime; };


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> Mesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USceneComponent> Muzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UPointLightComponent> PointLight;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UStaticMeshComponent> MuzzleFlashMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	TSubclassOf<class AActor> BulletDecalRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Bullet)
	TSubclassOf<class AActor> TracerRoundRef;



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float ReloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float BulletSpread;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 AmmoCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;


	UPROPERTY()
	bool bDoOnceReload;


	UFUNCTION()
	virtual void AmmoCheck() PURE_VIRTUAL(AVIWeaponbase::AmmoCheck,);

	UFUNCTION()
	virtual void LineTrace() PURE_VIRTUAL(AVIWeaponbase::LineTrace, );

	UFUNCTION()
	virtual void SpawnDecalTracer(FVector Location, FVector SpawnTransformLocation, FVector  ImpactPoint) PURE_VIRTUAL(AVIWeapons::SpawnDecalTracer, );

	UFUNCTION()
	virtual void MuzzleFlash() PURE_VIRTUAL(AVIWeaponbase::MuzzleFlash, );


};
