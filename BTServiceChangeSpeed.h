// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTServiceChangeSpeed.generated.h"

/**
 * 
 */
UCLASS()
class AITEST_API UBTServiceChangeSpeed : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTServiceChangeSpeed();

	//called when auxiliary node becomes active
	void OnBecomeRelevant(UBehaviorTreeComponent& OwnerComp, uint8 *NodeMemory) override;

protected:
	FString GetStaticServiceDescription()const override;
	
private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		float AISpeed = 600.0f;
};
