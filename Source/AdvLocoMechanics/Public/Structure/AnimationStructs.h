
#pragma once

#include "CoreMinimal.h"
#include "AnimationStructs.generated.h"


USTRUCT(BlueprintType)
struct FDirectionalAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimSequence* Forward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Backward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Right;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimSequence* Left;

};

USTRUCT(BlueprintType)
struct FGaitSetting
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float MaxWalkSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFrictionFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BrakingFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bUseSeparateBrakingFriction;

};