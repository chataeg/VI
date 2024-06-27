// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "PickUpAKWeapon.generated.h"

UCLASS()
class VI_API APickUpAKWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickUpAKWeapon();

	FORCEINLINE TObjectPtr<UStaticMeshComponent> GetStaticMesh() { return StaticMesh; };


protected:

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere,BlueprintReadOnly)
	TObjectPtr<USphereComponent> SphereCollision;



protected:

	UFUNCTION()
	virtual void BeginPlay() override;


	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
