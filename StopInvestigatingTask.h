// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "StopInvestigatingTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UStopInvestigatingTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UStopInvestigatingTask(FObjectInitializer const& ObjectInitializer);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
};
