// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VICharacterBase.h"
#include "InputActionValue.h"
#include "VICharacter.generated.h"

/**
 * 
 */
UCLASS()
class VI_API AVICharacter : public AVICharacterBase
{
	GENERATED_BODY()
	


public:
	AVICharacter();

public:
	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	
	FORCEINLINE bool GetbIsReloading() { return bIsReloading; };
	FORCEINLINE TObjectPtr<class UCameraComponent> GetCamera() { return Camera; };

	// Camera Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USpringArmComponent> CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UCameraComponent> Camera;

	// Mesh Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadwrite, Category = Mesh, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USkeletalMeshComponent> FirstPersonMesh;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PostProcess)
	
	TObjectPtr <class UPostProcessComponent> PostProcessComponent;
	

protected:
	// Weapon Section
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;

	UPROPERTY()

	bool bDoOnceReload;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)

	int32 Health;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> PrimaryWeaponBpRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AVIAKWeapon> PrimaryWeapon;

	// 블루프린트에서 에셋 지정
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> FireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> ReloadActionMontage;



	// Input Section

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> JumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> LookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> FireAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ReloadAction;


	// Input Action Functions

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	
	void Fire();
	
	void Reload();


	// BlueprintCallable Functions

	UFUNCTION(BlueprintCallable)
	void SetupCharacterControl();

	UFUNCTION(BlueprintCallable)
	void SpawnInitialPrimaryWeapon();




};
