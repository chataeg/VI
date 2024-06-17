// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/VITracerRound.h"
#include "Components/SphereComponent.h"
#include "GameFrameWork/ProjectileMovementComponent.h"

// Sets default values
AVITracerRound::AVITracerRound()
{
	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));

	Sphere->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));

	StaticMesh->SetupAttachment(Sphere);
	StaticMesh->SetRelativeScale3D(FVector(8.75f, 0.5f, 0.5f));
	

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ProjectileMovement->SetUpdatedComponent(RootComponent);

	ProjectileMovement->InitialSpeed = 7000.0f;
	ProjectileMovement->Bounciness = 0;
	ProjectileMovement->Friction = 0;
	ProjectileMovement->InterpLocationTime = 0.05f;
	
	// 일직선으로 총알을 날려보내기 위해 0.0f로 설정
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 12000.f;

	StaticMesh->OnComponentHit.AddDynamic(this, &AVITracerRound::OnHitStaticMesh);
	Sphere->OnComponentHit.AddDynamic(this, &AVITracerRound::OnHitSphere);

}

void AVITracerRound::OnHitStaticMesh( UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}

void AVITracerRound::OnHitSphere(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Destroy();
}


