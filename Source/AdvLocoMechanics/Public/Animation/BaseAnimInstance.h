
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Interface/AnimDataReceiverInterface.h"
#include "BaseAnimInstance.generated.h"


UCLASS()
class ADVLOCOMECHANICS_API UBaseAnimInstance : public UAnimInstance, public IAnimDataReceiverInterface
{
	GENERATED_BODY()
	
public:
	//AnimDataReceiverInterface
	virtual void ReceiveCurrentGait_Implementation(EGait Gait) override;
	virtual void ReceiveCurrentLocomotionState_Implementation(ELocomotionState LocomotionState) override;
	virtual void ReceiveGroundDistance_Implementation(float Distance) override;
};
