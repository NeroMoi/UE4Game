// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include"NPC.h"
#include"BehaviorTree/BehaviorTreeTypes.h"
#include "PatrolPointTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UPatrolPointTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UPatrolPointTask(FObjectInitializer const & ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
};
