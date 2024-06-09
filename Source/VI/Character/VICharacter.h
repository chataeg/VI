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
	// Weapon Section

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> PrimaryWeaponBpRef;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon , Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class AActor> PrimaryWeapon;








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
	TObjectPtr<class UInputAction> ShootAction;




	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);



	UFUNCTION(BlueprintCallable)
	void SetupCharacterControl();

	UFUNCTION(BlueprintCallable)
	void SpawnInitialPrimaryWeapon();




};
