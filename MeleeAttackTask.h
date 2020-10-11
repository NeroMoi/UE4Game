// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include"NPC.h"
#include "MeleeAttackTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UMeleeAttackTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UMeleeAttackTask();
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;

private:

	bool MontageHasFinished(ANPC * const Npc);

};

