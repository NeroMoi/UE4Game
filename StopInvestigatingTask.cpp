// Fill out your copyright notice in the Description page of Project Settings.


#include "StopInvestigatingTask.h"
#include "NPCAIController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"BlackboardKey.h"

UStopInvestigatingTask::UStopInvestigatingTask(FObjectInitializer const& ObjectInitializer)
{
	NodeName = TEXT("Stop Investigating");
}
EBTNodeResult::Type UStopInvestigatingTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	// get the npc controller and  write false to the blackboard key "IsInvestigating"

	auto const AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	AIController->GetBlackboard()->SetValueAsBool(GetSelectedBlackboardKey(), false);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
