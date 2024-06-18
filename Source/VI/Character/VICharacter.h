// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/VICharacterBase.h"
#include "InputActionValue.h"
#include "Components/TimelineComponent.h"
#include "VICharacter.generated.h"


DECLARE_DELEGATE_OneParam(FBindActionParam, const float);

/* 캐릭터 클래스 */
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
	

	// Weapon Section

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	bool bIsReloading;

	UPROPERTY()
	bool bDoOnceReload;


	UPROPERTY(VisibleAnywhere,BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bEquippedWeapon;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool bADS;



	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Health;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> PrimaryWeaponBpRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AVIAKWeapon> PrimaryWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> SecondaryWeaponBpRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AVIGlockWeapon> SecondaryWeapon;


	// 블루프린트에서 에셋 지정

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKFireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKADSFireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> AKReloadActionMontage;




	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GlockFireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GlockADSFireActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> GlockReloadActionMontage;





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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipFirstAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> EquipSecondAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UInputAction> ADSAction;


	// TimeLine Section


	FOnTimelineFloat ADSCallback;
	FOnTimelineEvent ADSTimelineFinish;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Timeline)
	TObjectPtr<class UTimelineComponent> ADSTimeline;

	UPROPERTY(EditAnywhere,BlueprintReadOnly,Category = Timeline)
	TObjectPtr<class UCurveFloat> ADSCurve;




	// Input Action Functions

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);

	void Fire();
	
	void Reload();

	void EquipFirst();

	void EquipSecond();


	UFUNCTION()
	void ADSTimeLineFunc(float value);

	UFUNCTION()
	void StartADS();
	





	// BlueprintCallable Functions

	UFUNCTION(BlueprintCallable)
	void SetupCharacterControl();

	UFUNCTION(BlueprintCallable)
	void SpawnInitialPrimaryWeapon();




};
