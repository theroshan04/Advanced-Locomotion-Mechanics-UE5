#pragma once

#include "CoreMinimal.h"
#include "AnimationEnums.generated.h"

UENUM(BlueprintType)
enum class EGait : uint8
{
	Jogging     UMETA(DisplayName = "Jogging"),
	Walking     UMETA(DisplayName = "Walking"),
	Crouching   UMETA(DisplayName = "Crouching")
};

UENUM(BlueprintType)
enum class ELocomotionDirection : uint8
{
	Forward   UMETA(DisplayName = "Forward"),
	Backward  UMETA(DisplayName = "Backward"),
	Right     UMETA(DisplayName = "Right"),
	Left      UMETA(DisplayName = "Left")
};

UENUM(BlueprintType)
enum class ELocomotionState : uint8
{
	Unarmed  UMETA(DisplayName = "Unarmed"),
	Rifle    UMETA(DisplayName = "Rifle"),
	Pistol   UMETA(DisplayName = "Pistol"),
	Shotgun  UMETA(DisplayName = "Shotgun")
};

UENUM(BlueprintType)
enum class ERootYawOffsetMode : uint8
{
	Accumulate   UMETA(DisplayName = "Accumulate"),
	BlendOut     UMETA(DisplayName = "BlendOut"),
	Hold         UMETA(DisplayName = "Hold")
};