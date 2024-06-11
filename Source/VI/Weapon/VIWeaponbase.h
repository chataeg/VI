// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimationAsset.h"
#include "VIWeaponbase.generated.h"

UCLASS(Abstract)
class VI_API AVIWeaponbase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVIWeaponbase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float ReloadTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	float BulletSpread;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 AmmoCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Meta = (AllowPrivateAccess = "true"))
	int32 MaxAmmo;

	
	UFUNCTION()
	virtual void AmmoCheck() PURE_VIRTUAL(AVIWeaponbase::AmmoCheck,);

	UFUNCTION()
	virtual void LineTrace() PURE_VIRTUAL(AVIWeaponbase::LineTrace, );





};
