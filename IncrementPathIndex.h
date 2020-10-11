// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include"BehaviorTree/BehaviorTreeTypes.h"
#include "IncrementPathIndex.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class AITEST_API UIncrementPathIndex : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	UIncrementPathIndex(FObjectInitializer const& ObjctInitializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory) override;

private:

	enum class EDirectionType
	{
		Forward,
		Reverse
	};

	EDirectionType direction = EDirectionType::Forward;


	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		bool Bidirection = true;
};

