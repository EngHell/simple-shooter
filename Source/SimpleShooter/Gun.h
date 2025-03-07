﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AGun();

private:
	UPROPERTY(EditAnywhere, Category="Gun")
	float MaxRange = 1000.f;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USceneComponent* Root;
	UPROPERTY(EditDefaultsOnly, Category="Components")
	USkeletalMeshComponent* Mesh;
	UPROPERTY(EditDefaultsOnly, Category="Particles")
	UParticleSystem* MuzzleFlashParticleSystem;
	UPROPERTY(EditDefaultsOnly, Category="Particles")
	UParticleSystem* ImpactEffectParticleSystem;
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void PullTrigger();
};
