// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Gun.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AShooterCharacter::MoveAround(const FInputActionValue& Value)
{
	FVector2D MovementValue = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementValue.Y);
	AddMovementInput(GetActorRightVector(), MovementValue.X);
}

void AShooterCharacter::LookAround(const FInputActionValue& Value)
{
	FVector2D MouseValue = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	// -1 to remove inversion.
	AddControllerPitchInput(MouseValue.Y * -1 * RotationRate * DeltaTime);
	AddControllerYawInput(MouseValue.X * RotationRate * DeltaTime);
}

void AShooterCharacter::LookAroundController(const FInputActionValue& Value)
{
	FVector2D MouseValue = Value.Get<FVector2D>();
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	// -1 to remove inversion.
	AddControllerPitchInput(MouseValue.Y * ControllerRotationRate * DeltaTime);
	AddControllerYawInput(MouseValue.X * ControllerRotationRate * DeltaTime);
}

void AShooterCharacter::JumpCallback(const FInputActionValue& Value)
{
	ACharacter::Jump();
}

void AShooterCharacter::PullTrigger(const FInputActionValue& Value)
{
	if(!Gun)
	{
		return;
	}

	Gun->PullTrigger();
}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

	Gun = GetWorld()->SpawnActor<AGun>(GunClass, GetActorLocation(), GetActorRotation());
	GetMesh()->HideBoneByName(TEXT("weapon_r"), PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	auto PC = Cast<APlayerController>(GetController());
	auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());
	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
	
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	Input->BindAction(MoveForwardAction, ETriggerEvent::Triggered, this, &AShooterCharacter::MoveAround);
	Input->BindAction(LookUpAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookAround);
	Input->BindAction(LookAroundControllerAction, ETriggerEvent::Triggered, this, &AShooterCharacter::LookAroundController);
	Input->BindAction(JumpAction, ETriggerEvent::Started, this, &AShooterCharacter::JumpCallback);
	Input->BindAction(ShootAction, ETriggerEvent::Started, this, &AShooterCharacter::PullTrigger);
}

