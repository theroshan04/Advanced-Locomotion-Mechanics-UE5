
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enumeration/AnimationEnums.h"
#include "Structure/AnimationStructs.h"
#include "PlayerCharacter.generated.h"

class USkeletalMeshComponent;
class UCapsuleComponent;
class USpringArmComponent;
class UCameraComponent;
class UCharacterMovementComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UCLASS()
class ADVLOCOMECHANICS_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintCallable)
	void UpdateLocomotionState(ELocomotionState State);

protected:
	virtual void BeginPlay() override;
	UFUNCTION()
	void UpdateGaitSettings(EGait Gait);

private:
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void Walk();
	void CustomCrouch();
	void OpenMenu();
	void Aim();
	void Shoot();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "References")
	APlayerController* PlayerController;

	UPROPERTY(BlueprintReadOnly, Category = "Locomotion")
	UAnimInstance* PlayerAnimInstance;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Locomotion")
	EGait CurrentGait;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	ELocomotionState CurrentLocomotionState;

	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = "Locomotion")
	TMap<EGait, FGaitSetting> GaitSettingMap;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TSubclassOf<UAnimInstance> UnarmedAnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TSubclassOf<UAnimInstance> RifleAnimInstanceClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Locomotion")
	TSubclassOf<UAnimInstance> PistolAnimInstanceClass;

private:
	UPROPERTY()
	USkeletalMeshComponent* PlayerMesh;

	UPROPERTY()
	UCapsuleComponent* PlayerCapsule;

	UPROPERTY()
	UCharacterMovementComponent* PlayerMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;
	
	//Inputs
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* WalkAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenMenuAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* AimAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputAction* ShootAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MappingContext;

};
