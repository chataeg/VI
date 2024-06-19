// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/VICharacter.h"

#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Weapon/VIWeaponbase.h"
#include "Weapon/VIAKWeapon.h"
#include "Weapon/VIGlockWeapon.h"

#include "VI/Player/VIPlayerController.h"
#include "VI.h"

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
		
		Gun = CreateDefaultSubobject<UChildActorComponent>(TEXT("Gun"));
		Gun->SetChildActorClass(WeaponBaseRef.Class);
		Gun->SetupAttachment(FirstPersonMesh, FName(TEXT("Palm_R")));
	}
	

	bIsReloading = false;
	bEquippedWeapon = true;
	bWeaponEquipped = 0;
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
	//ADSTimelineFinish.BindUFunction(this, FName("EndADS"));
	



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
	
	//Cast<AVIWeaponbase>(Gun->GetChildActor())->Fire();

}

void AVICharacter::Reload()
{
	if (bEquippedWeapon)
		PrimaryWeapon->Reload();
	else
		SecondaryWeapon->Reload();
	
}

void AVICharacter::EquipFirst()
{
	
	if (!bIsReloading)
	{
		bWeaponEquipped = 0;
		switch (bWeaponEquipped)
		{ 
			case 0:
				Gun->SetChildActorClass(AKWeaponBpRef);
				D("Equipped AK")
					break;
			case 1:
				Gun->SetChildActorClass(GlockWeaponBpRef);
				D("Equipped Glock")
					break;
		}
	}
	
		//PrimaryWeapon->GetRootComponent()->SetVisibility(true);
		//->GetRootComponent()->SetVisibility(false);
	

}

void AVICharacter::EquipSecond()
{
	if (!bIsReloading)
	{
		bWeaponEquipped = 1;
		switch (bWeaponEquipped)
		{
			case 0:
				Gun->SetChildActorClass(AKWeaponBpRef);
				D("Equipped AK")
					break;
			case 1:
				Gun->SetChildActorClass(GlockWeaponBpRef);
				D("Equipped Glock")
					break;
		}
	}
	
	//	PrimaryWeapon->GetRootComponent()->SetVisibility(false);
	//	SecondaryWeapon->GetRootComponent()->SetVisibility(true);
	

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
