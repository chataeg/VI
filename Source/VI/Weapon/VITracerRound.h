// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VITracerRound.generated.h"

UCLASS()
class VI_API AVITracerRound : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AVITracerRound();



protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr <class USphereComponent> Sphere;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr <class UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TObjectPtr <class UProjectileMovementComponent> ProjectileMovement;

	UFUNCTION()
	void OnHitStaticMesh(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
	void OnHitSphere(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);




};
