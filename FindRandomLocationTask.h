// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include"GameFramework/Pawn.h"
#include"BehaviorTree/BehaviorTreeComponent.h"
#include"BehaviorTree/BehaviorTreeTypes.h"
#include"UObject/UObjectGlobals.h"
#include "FindRandomLocationTask.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UFindRandomLocationTask : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UFindRandomLocationTask(FObjectInitializer const & ObjectInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory);

private:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Search", meta = (AllowPrivateAccess = "true"))
		float SearchRadius = 1500.0f;


};

