// Fill out your copyright notice in the Description page of Project Settings.


#include "IncrementPathIndex.h"
#include"NPCAIController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"NPC.h"
#include"BlackboardKey.h"
#include<cmath>

UIncrementPathIndex::UIncrementPathIndex(FObjectInitializer const& ObjctInitializer)
{
	NodeName = TEXT("IncrementPathIndex");
}
EBTNodeResult::Type UIncrementPathIndex::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANPCAIController* const  AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	auto const NPC = Cast<ANPC>(AIController->GetPawn());

	auto const NumOfPoints = NPC->GetPatrolPath()->GetPartolNum();

	int MinIndex = 0;
	int MaxIndex = NumOfPoints -1;


	// get and set the blackboard index key

	int CurrentIndex = AIController->GetBlackboard()->GetValueAsInt(BB_KEYS::PatrolPathIndex);


	if (Bidirection)
	{
		if (CurrentIndex >= MaxIndex && direction == EDirectionType::Forward)
		{

			direction = EDirectionType::Reverse;
		}
		else if (CurrentIndex == MinIndex && direction == EDirectionType::Reverse)
		{

			direction = EDirectionType::Forward;
		}
	}


	AIController->GetBlackboard()->SetValueAsInt(BB_KEYS::PatrolPathIndex, (direction == EDirectionType::Forward ? std::abs(++CurrentIndex) : std::abs(--CurrentIndex))% NumOfPoints);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
