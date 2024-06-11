// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/VIGameMode.h"
#include "GameFrameWork/HUD.h"

AVIGameMode::AVIGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/BP_VICharacter.BP_VICharacter_C'"));
	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}


	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerClassRef(TEXT("/Script/CoreUObject.Class'/Script/VI.VIPlayerController'"));
	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<AHUD> HUDClassRef(TEXT("/Script/Engine.Blueprint'/Game/VI/Character/Blueprint/HUD/BP_HUD.BP_HUD_C'"));
	if (HUDClassRef.Class)
	{
		HUDClass = HUDClassRef.Class;
	}



}
