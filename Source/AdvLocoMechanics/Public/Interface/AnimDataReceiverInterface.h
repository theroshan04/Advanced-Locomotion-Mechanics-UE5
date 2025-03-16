
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Enumeration/AnimationEnums.h"
#include "AnimDataReceiverInterface.generated.h"

UINTERFACE(MinimalAPI)
class UAnimDataReceiverInterface : public UInterface
{
	GENERATED_BODY()
};

class ADVLOCOMECHANICS_API IAnimDataReceiverInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, Category = "AnimDataReceiver")
	void ReceiveCurrentGait(EGait Gait);

	UFUNCTION(BlueprintNativeEvent, Category = "AnimDataReceiver")
	void ReceiveCurrentLocomotionState(ELocomotionState LocomotionState);

	UFUNCTION(BlueprintNativeEvent, Category = "AnimDataReceiver")
	void ReceiveGroundDistance(float Distance);

};
