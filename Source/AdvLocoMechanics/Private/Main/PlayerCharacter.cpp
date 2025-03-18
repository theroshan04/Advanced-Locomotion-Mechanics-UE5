
#include "Main/PlayerCharacter.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "EnhancedInputComponent.h"
#include "Main/MainPlayerController.h"
#include "Interface/AnimDataReceiverInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/World.h"


APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	PlayerCapsule = GetCapsuleComponent();
	PlayerCapsule->InitCapsuleSize(32.0f, 90.0f);

	PlayerMesh = GetMesh();
	PlayerMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -90.0f));
	PlayerMesh->SetRelativeRotation(FRotator(-90.0f, 0.0f, 0.0f));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(PlayerCapsule);
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	SpringArm->TargetArmLength = 700.0f;
	SpringArm->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);

	PlayerMovement = GetCharacterMovement();
	PlayerMovement->bOrientRotationToMovement = false;
	PlayerMovement->bUseControllerDesiredRotation = false;
	PlayerMovement->NavAgentProps.bCanCrouch = true;
	PlayerMovement->CrouchedHalfHeight = 60.0f;
	PlayerMovement->JumpZVelocity = 700.0f;
	PlayerMovement->BrakingDecelerationFalling = 1500.0f;
	PlayerMovement->AirControl = 0.35f;

	GaitSettingMap.Add(EGait::Walking, { 250.0f,250.0f,250.0f,1.0f,0.0f,true });
	GaitSettingMap.Add(EGait::Jogging, { 800.0f,800.0f,1200.0f,1.0f,0.0f,true });
	GaitSettingMap.Add(EGait::Crouching, { 250.0f,250.0f,250.0f,1.0f,0.0f,true });

}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<AMainPlayerController>(GetController());
	if (PlayerController)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MappingContext, 0);
		}
	}

	PlayerAnimInstance = PlayerMesh->GetAnimInstance();
	UpdateGaitSettings(EGait::Jogging);
	UpdateLocomotionState(ELocomotionState::Unarmed);

}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GetGroundDistance();
}

void APlayerCharacter::UpdateGaitSettings(EGait Gait)
{
	const FGaitSetting* FoundGaitSettings = GaitSettingMap.Find(Gait);
	CurrentGait = Gait;
	IAnimDataReceiverInterface::Execute_ReceiveCurrentGait(PlayerAnimInstance, CurrentGait);
	PlayerMovement->MaxWalkSpeed = FoundGaitSettings->MaxWalkSpeed;
	PlayerMovement->MaxAcceleration = FoundGaitSettings->MaxAcceleration;
	PlayerMovement->BrakingDecelerationWalking = FoundGaitSettings->BrakingDeceleration;
	PlayerMovement->BrakingFrictionFactor = FoundGaitSettings->BrakingFrictionFactor;
	PlayerMovement->BrakingFriction = FoundGaitSettings->BrakingFriction;
	PlayerMovement->bUseSeparateBrakingFriction = FoundGaitSettings->bUseSeparateBrakingFriction;
}

void APlayerCharacter::UpdateLocomotionState(ELocomotionState State)
{
	CurrentLocomotionState = State;
	IAnimDataReceiverInterface::Execute_ReceiveCurrentLocomotionState(PlayerAnimInstance, CurrentLocomotionState);
	switch (State)
	{
	case ELocomotionState::Unarmed:
		PlayerAnimInstance->LinkAnimClassLayers(UnarmedAnimInstanceClass);
		break;
	case ELocomotionState::Rifle:
		PlayerAnimInstance->LinkAnimClassLayers(RifleAnimInstanceClass);
		break;
	case ELocomotionState::Pistol:
		PlayerAnimInstance->LinkAnimClassLayers(PistolAnimInstanceClass);
		break;
	default:
		break;
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	AddControllerPitchInput(LookAxisVector.Y);
	AddControllerYawInput(LookAxisVector.X);
}

void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void APlayerCharacter::Walk()
{
	switch (CurrentGait)
	{
	case EGait::Jogging:
		UpdateGaitSettings(EGait::Walking);
		break;
	case EGait::Walking:
		UpdateGaitSettings(EGait::Jogging);
		break;
	case EGait::Crouching:
	default:
		break;
	}
}

void APlayerCharacter::CustomCrouch()
{
	switch (CurrentGait)
	{
	case EGait::Walking:
	case EGait::Jogging:
		UpdateGaitSettings(EGait::Crouching);
		Crouch();
		break;
	case EGait::Crouching:
		UpdateGaitSettings(EGait::Jogging);
		UnCrouch();
		break;
	default:
		break;
	}
}

void APlayerCharacter::OpenMenu()
{
}

void APlayerCharacter::Aim()
{
}

void APlayerCharacter::Shoot()
{
}

void APlayerCharacter::GetGroundDistance()
{
	FVector StartLocation = GetActorLocation() - FVector(0.0f, 0.0f, PlayerCapsule->GetScaledCapsuleHalfHeight());
	FVector EndLocation = GetActorLocation() - FVector(0.0f, 0.0f, 1000.0f);
	FHitResult HitResult;
	bool bHit = UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartLocation, EndLocation, 5.0f, UEngineTypes::ConvertToTraceType(ECC_Visibility), false, { this }, EDrawDebugTrace::None, HitResult, true);
	if (bHit && PlayerAnimInstance)
	{
		GroundDistance = HitResult.Distance;
		IAnimDataReceiverInterface::Execute_ReceiveGroundDistance(PlayerAnimInstance, GroundDistance);
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(WalkAction, ETriggerEvent::Started, this, &APlayerCharacter::Walk);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::CustomCrouch);
		EnhancedInputComponent->BindAction(OpenMenuAction, ETriggerEvent::Started, this, &APlayerCharacter::OpenMenu);
		EnhancedInputComponent->BindAction(AimAction, ETriggerEvent::Started, this, &APlayerCharacter::Aim);
		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Started, this, &APlayerCharacter::Shoot);
	}

}




