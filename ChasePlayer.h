// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include"BehaviorTree/BehaviorTreeTypes.h"
#include"NPC.h"
#include "ChasePlayer.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UChasePlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:

	UChasePlayer(FObjectInitializer const& ObjctInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;
	
};
