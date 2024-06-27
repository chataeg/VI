// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VICharacter.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Weapon/VIWeaponbase.h"
#include "Weapon/VIAKWeapon.h"
#include "Weapon/VIGlockWeapon.h"

#include "Weapon/PickUpAKWeapon.h"
#include "Weapon/PickUpGlockWeapon.h"


#include "VI/Player/VIPlayerController.h"
#include "VI.h"
#include "GameData/VIGunData.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/PostProcessComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/ChildActorComponent.h"



AVICharacter::AVICharacter()
{
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(RootComponent);
	Camera->bUsePawnControlRotation = true;
	Camera->SetRelativeLocation(FVector(-10.0f, 0.0f, 60.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(Camera);
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;

		
	
	static ConstructorHelpers::FClassFinder<AVIWeaponbase> WeaponBaseRef(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_Weaponbase.BP_Weaponbase_C'"));
	if (WeaponBaseRef.Class)
	{
		WeaponBaseBpRef = WeaponBaseRef.Class;

		Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Gun"));
		Gun->SetChildActorClass(WeaponBaseRef.Class);
		Gun->SetupAttachment(FirstPersonMesh, FName(TEXT("Palm_R")));
		
	}
	

	bIsReloading = false;
	bEquippedWeapon = true;
	WeaponEquipped = 0;
	bADS = false;

	Health = 100;
	


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> FirstPersonMeshRef(TEXT("/Script/Engine.SkeletalMesh'/Game/WeaponAssets/RiggedMeshes/FPSArms_rigged.FPSArms_rigged'"));
	if (FirstPersonMeshRef.Object)
	{
		FirstPersonMesh->SetSkeletalMesh(FirstPersonMeshRef.Object);
	}

	FirstPersonMesh->SetRelativeLocation(FVector(1.6f, 7.8f, -23.6f));
	FirstPersonMesh->SetRelativeRotation(FRotator(0.0f, 6.4f, 0.0f));


	FirstPersonMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	PostProcessComponent = CreateDefaultSubobject<UPostProcessComponent>(TEXT("PostProcessComponent"));
	PostProcessComponent->SetupAttachment(RootComponent);

	ThrowWeaponSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ThrowPoint"));
	ThrowWeaponSceneComponent->SetupAttachment(RootComponent);
	ThrowWeaponSceneComponent->SetRelativeLocation(FVector(80.0f,0.0f , 37.7f));


	PrimaryWeapon.Class = WeaponBaseBpRef;
	SecondaryWeapon.Class = WeaponBaseBpRef;



	
	// 애니메이션 블루프린트 로딩 시 크래시가 일어나 블루프린트에서 로딩합니다.

	/*
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimBPRef(TEXT("/Script/Engine.AnimBlueprint'/Game/WeaponAssets/Animations/ABP_Arms.ABP_Arms_C'"));
	if (AnimBPRef.Succeeded())
	{
		
		FirstPersonMesh->SetAnimInstanceClass(AnimBPRef.Class);

	}
	*/


	// ADS Timeline Section


	ADSTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ADSTimeline"));
	ADSCallback.BindUFunction(this, FName("ADSTimeLineFunc"));

	// Constructor Section
	
	static ConstructorHelpers::FClassFinder<AActor> AKWeaponBp(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_AKWeapon.BP_AKWeapon_C'"));
	if (AKWeaponBp.Succeeded())
	{
		AKWeaponBpRef = AKWeaponBp.Class;
	}
	
	static ConstructorHelpers::FClassFinder<AActor> GlockWeaponBp(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_GlockWeapon.BP_GlockWeapon_C'"));
	if (GlockWeaponBp.Succeeded())
	{
		GlockWeaponBpRef = GlockWeaponBp.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> PickupAKWeaponBp(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_PickUpAKWeapon.BP_PickUpAKWeapon_C'"));
	if (PickupAKWeaponBp.Succeeded())
	{
		PickUpAKWeaponBpRef = PickupAKWeaponBp.Class;
	}

	static ConstructorHelpers::FClassFinder<AActor> PickUpGlockWeaponBp(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/Weapon/BP_PickUpGlockWeapon.BP_PickUpGlockWeapon_C'"));
	if (PickUpGlockWeaponBp.Succeeded())
	{
		PickUpGlockWeaponBpRef = PickUpGlockWeaponBp.Class;
	}


	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT("/Script/EnhancedInput.InputMappingContext'/Game/VI/Character/Input/IMC_Default.IMC_Default'"));
	if (nullptr != InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_Move.IA_Move'"));
	if (nullptr != InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_Jump.IA_Jump'"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_Look.IA_Look'"));
	if (nullptr != InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionFireRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_Fire.IA_Fire'"));
	if (nullptr != InputActionFireRef.Object)
	{
		FireAction = InputActionFireRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionReloadRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_Reload.IA_Reload'"));
	if (nullptr != InputActionReloadRef.Object)
	{
		ReloadAction = InputActionReloadRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipFirstRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_EquipFirst.IA_EquipFirst'"));
	if (nullptr != InputActionEquipFirstRef.Object)
	{
		EquipFirstAction = InputActionEquipFirstRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEquipSecondRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_EquipSecond.IA_EquipSecond'"));
	if (nullptr != InputActionEquipSecondRef.Object)
	{
		EquipSecondAction = InputActionEquipSecondRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionADSActionRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_ADS.IA_ADS'"));
	if (nullptr != InputActionADSActionRef.Object)
	{
		ADSAction = InputActionADSActionRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSetupFirstWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_SetupFirst.IA_SetupFirst'"));
	if (nullptr != InputActionSetupFirstWeaponRef.Object)
	{
		SetupFirstWeaponAction = InputActionSetupFirstWeaponRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSetupSecondWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_SetupSecond.IA_SetupSecond'"));
	if (nullptr != InputActionSetupSecondWeaponRef.Object)
	{
		SetupSecondWeaponAction = InputActionSetupSecondWeaponRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionThrowWeaponRef(TEXT("/Script/EnhancedInput.InputAction'/Game/VI/Character/Input/Actions/IA_ThrowWeapon.IA_ThrowWeapon'"));
	if (nullptr != InputActionThrowWeaponRef.Object)
	{
		ThrowWeaponAction = InputActionThrowWeaponRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UCurveFloat> ADSCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/VI/Character/Blueprint/Curve/Curve_ADS.Curve_ADS'"));
	if (nullptr != ADSCurveRef.Object)
	{
		ADSCurve = ADSCurveRef.Object;
	}
	

}

void AVICharacter::BeginPlay()
{
	Super::BeginPlay();

	/* 블루프린트에서 SetUpCharacterControl - SpawnInitialPrimaryWeapon 호출됩니다. */

	// ADSTimeline
	
	ADSTimeline->AddInterpFloat(ADSCurve, ADSCallback);
	ADSTimeline->SetTimelineLength(0.15f);
	ADSTimeline->SetLooping(false);
	//ADSTimeline->SetTimelineFinishedFunc(ADSTimelineFinish);




}

void AVICharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AVICharacter::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AVICharacter::Look);

	EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &AVICharacter::Fire);
	EnhancedInputComponent->BindAction(ReloadAction, ETriggerEvent::Triggered, this, &AVICharacter::Reload);

	EnhancedInputComponent->BindAction(EquipFirstAction, ETriggerEvent::Triggered, this, &AVICharacter::EquipFirst);
	EnhancedInputComponent->BindAction(EquipSecondAction, ETriggerEvent::Triggered, this, &AVICharacter::EquipSecond);

	EnhancedInputComponent->BindAction(SetupFirstWeaponAction, ETriggerEvent::Triggered, this, &AVICharacter::SetupFirst);
	EnhancedInputComponent->BindAction(SetupSecondWeaponAction, ETriggerEvent::Triggered, this, &AVICharacter::SetupSecond);
	
	EnhancedInputComponent->BindAction(ThrowWeaponAction, ETriggerEvent::Triggered, this, &AVICharacter::ThrowWeapon);



	EnhancedInputComponent->BindAction(ADSAction, ETriggerEvent::Triggered, this, &AVICharacter::StartADS);
	

}

void AVICharacter::SetupCharacterControl()
{
	if (!IsLocallyControlled()) return;
	
		APlayerController* PlayerController = CastChecked<APlayerController>(GetController());

		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			if (Subsystem)
			{
				Subsystem->AddMappingContext(DefaultMappingContext, 0);
				//Subsystem->RemoveMappingContext(DefaultMappingContext);

			}
		}
	
}

void AVICharacter::SpawnInitialPrimaryWeapon()
{

	/*
	if (UWorld* World = GetWorld())
	{
		if (AKWeaponBpRef != nullptr)
		{
			// 스폰할 위치와 회전을 지정
			FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
			FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

			// 액터 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 월드에서 액터를 스폰
			PrimaryWeapon = Cast<AVIAKWeapon>(World->SpawnActor<AActor>(AKWeaponBpRef, SpawnLocation, SpawnRotation, SpawnParams));

			if (PrimaryWeapon)
			{
				// 스폰된 액터에 대해 추가 작업을 수행
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true); 

				PrimaryWeapon->AttachToComponent(FirstPersonMesh, AttachmentRules, FName(TEXT("Palm_R")));
				 
			}

		}

		if (GlockWeaponBpRef != nullptr)
		{
			// 스폰할 위치와 회전을 지정
			FVector SpawnLocation = FVector(0.0f, 0.0f, 0.0f);
			FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

			// 액터 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 월드에서 액터를 스폰
			SecondaryWeapon = Cast<AVIGlockWeapon>(World->SpawnActor<AActor>(GlockWeaponBpRef, SpawnLocation, SpawnRotation, SpawnParams));

			if (SecondaryWeapon)
			{
				// 스폰된 액터에 대해 추가 작업을 수행
				FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);

				SecondaryWeapon->AttachToComponent(FirstPersonMesh, AttachmentRules, FName(TEXT("Palm_R")));
				
				SecondaryWeapon->GetRootComponent()->SetVisibility(false);
			}

		}

	
	}
	*/

}

void AVICharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	AVIPlayerController* PC = Cast<AVIPlayerController>(GetController());

	// 키 동시입력 제한
	if (PC)
	{
		if (PC->IsInputKeyDown(EKeys::W) && PC->IsInputKeyDown(EKeys::S)) return;
		if (PC->IsInputKeyDown(EKeys::A) && PC->IsInputKeyDown(EKeys::D)) return;
	}


	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AVICharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	


	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AVICharacter::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("Fire"));

	if (bIsReloading) return;
	
	Cast<AVIWeaponbase>(Gun->GetChildActor())->Fire();

}

void AVICharacter::Reload()
{
	Cast<AVIWeaponbase>(Gun->GetChildActor())->Reload();

}

void AVICharacter::EquipFirst()
{
	D("EquipFirst")

	if (PrimaryWeapon.Class != WeaponBaseBpRef || SecondaryWeapon.Class != WeaponBaseBpRef)
	{
		AVIWeaponbase* CastedGunRef = Cast<AVIWeaponbase>(Gun->GetChildActor());
		
		if (!bIsReloading)
		{
			WeaponEquipped = 0;
				
			CastedGunRef->UnEquip();

			Gun->SetChildActorClass(AKWeaponBpRef);

			DF("Secondary Weapon %d", (SecondaryWeapon.Class == GlockWeaponBpRef) ? 1 : 0)

			if (PrimaryWeapon.Class == WeaponBaseBpRef)
			{
				Gun->SetChildActorClass(WeaponBaseBpRef);
				FirstPersonMesh->SetVisibility(false);
			}
			else
			{
				CastedGunRef->SetAmmoCount(PrimaryWeapon.AmmoCount);
				CastedGunRef->SetMaxAmmo(PrimaryWeapon.MaxAmmo);
				CastedGunRef->SetBulletSpread(PrimaryWeapon.BulletSpread);
				CastedGunRef->SetReloadTime(PrimaryWeapon.ReloadTime);
				FirstPersonMesh->SetVisibility(true);

				CastedGunRef->Equip();		
			}
		}
	}

}

void AVICharacter::EquipSecond()
{

	D("EquipSecond")

		if (PrimaryWeapon.Class != WeaponBaseBpRef || SecondaryWeapon.Class != WeaponBaseBpRef)
		{
			AVIWeaponbase* CastedGunRef = Cast<AVIWeaponbase>(Gun->GetChildActor());
		
			if (!bIsReloading)
			{
				WeaponEquipped = 1;
			
				CastedGunRef->UnEquip();

				Gun->SetChildActorClass(GlockWeaponBpRef);

				DF("Secondary Weapon %d", (SecondaryWeapon.Class == GlockWeaponBpRef) ? 1 : 0)

				if (SecondaryWeapon.Class == WeaponBaseBpRef)
				{
					Gun->SetChildActorClass(WeaponBaseBpRef);
					FirstPersonMesh->SetVisibility(false);
					D("SecondaryWeapon is empty")
				}
				else
				{
					CastedGunRef->SetAmmoCount(SecondaryWeapon.AmmoCount);
					CastedGunRef->SetMaxAmmo(SecondaryWeapon.MaxAmmo);
					CastedGunRef->SetBulletSpread(SecondaryWeapon.BulletSpread);
					CastedGunRef->SetReloadTime(SecondaryWeapon.ReloadTime);
					FirstPersonMesh->SetVisibility(true);
				}
			}
		}
}

void AVICharacter::SetupFirst()
{
	D("SetupFirst")
	PrimaryWeapon.Class = AKWeaponBpRef;
	PrimaryWeapon.AmmoCount = 30;
	PrimaryWeapon.MaxAmmo = 30;
	PrimaryWeapon.ReloadTime = 2.0f;
	PrimaryWeapon.BulletSpread = 2000.0f;

	DF("Secondary Weapon %d", (SecondaryWeapon.Class == GlockWeaponBpRef) ? 1 : 0)

}

void AVICharacter::SetupSecond()
{
	D("SetupSecond")
	SecondaryWeapon.Class = GlockWeaponBpRef;
	SecondaryWeapon.AmmoCount = 11;
	SecondaryWeapon.MaxAmmo = 11;
	SecondaryWeapon.ReloadTime = 1.5f;
	SecondaryWeapon.BulletSpread = 1500.0f;
}

void AVICharacter::ThrowWeapon()
{
	switch (WeaponEquipped)
	{
	case 0:
		if (PrimaryWeapon.Class == AKWeaponBpRef)
		{	
			// 스폰할 위치와 회전을 지정
			FVector SpawnLocation = ThrowWeaponSceneComponent->GetComponentLocation();
			FRotator SpawnRotation = FRotator(FMath::RandRange(-180.0f,180.0f), FMath::RandRange(-180.0f, 180.0f), FMath::RandRange(-180.0f, 180.0f));

			// 액터 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 월드에서 액터를 스폰
			APickUpAKWeapon* ThrownWeapon = GetWorld()->SpawnActor<APickUpAKWeapon>(PickUpAKWeaponBpRef, SpawnLocation, SpawnRotation, SpawnParams);
			
			ThrownWeapon->GetStaticMesh()->AddImpulse(ThrowWeaponSceneComponent->GetForwardVector() * 500.0f, TEXT("None"), true);

			PrimaryWeapon.Class = WeaponBaseBpRef;
			PrimaryWeapon.AmmoCount = 0;
			PrimaryWeapon.MaxAmmo = 0;
			PrimaryWeapon.ReloadTime = 0;
			PrimaryWeapon.BulletSpread = 0;

			Gun->SetChildActorClass(WeaponBaseBpRef);

			FirstPersonMesh->SetVisibility(false);




				
		}


		break;
	case 1:
		if (SecondaryWeapon.Class == GlockWeaponBpRef)
		{
			// 스폰할 위치와 회전을 지정
			FVector SpawnLocation = ThrowWeaponSceneComponent->GetComponentLocation();
			FRotator SpawnRotation = FRotator(FMath::RandRange(-180.0f, 180.0f), FMath::RandRange(-180.0f, 180.0f), FMath::RandRange(-180.0f, 180.0f));

			// 액터 스폰 파라미터 설정
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			// 월드에서 액터를 스폰
			APickUpGlockWeapon* ThrownWeapon = GetWorld()->SpawnActor<APickUpGlockWeapon>(PickUpGlockWeaponBpRef, SpawnLocation, SpawnRotation, SpawnParams);

			ThrownWeapon->GetStaticMesh()->AddImpulse(ThrowWeaponSceneComponent->GetForwardVector() * 500.0f, TEXT("None"), true);

			SecondaryWeapon.Class = WeaponBaseBpRef;
			SecondaryWeapon.AmmoCount = 0;
			SecondaryWeapon.MaxAmmo = 0;
			SecondaryWeapon.ReloadTime = 0;
			SecondaryWeapon.BulletSpread = 0;

			Gun->SetChildActorClass(WeaponBaseBpRef);

			FirstPersonMesh->SetVisibility(false);
		}

		break;

	}



}

void AVICharacter::ADSTimeLineFunc(float value)
{
	//D("TimeLine")
	if (bEquippedWeapon)
	{
		GetCamera()->SetFieldOfView(FMath::Lerp(90.0f, 45.0f, value));
		FirstPersonMesh->SetRelativeLocation(FMath::Lerp(FVector(1.6f, 7.8f, -23.6f), FVector(1.6f, 0.0f, -16.5f), value));
		FirstPersonMesh->SetRelativeRotation(FRotator(0.0f, -1.0f, 0.0f));

	}
	else
	{
		GetCamera()->SetFieldOfView(FMath::Lerp(90.0f, 45.0f, value));
		FirstPersonMesh->SetRelativeLocation(FMath::Lerp(FVector(1.6f, 7.8f, -23.6f), FVector(1.6f, 0.0f, -16.0f), value));
		FirstPersonMesh->SetRelativeRotation(FRotator(0.0f, -1.0f, 0.0f));

	}
}

void AVICharacter::StartADS()
{
	if (!bADS)
	{
		D("StartADS")
			bADS = true;
		ADSTimeline->Play();

	}
	else
	{
		D("EndADS")
			bADS = false;
		ADSTimeline->ReverseFromEnd();
	}
	

}
