// Fill out your copyright notice in the Description page of Project Settings.


#include "PatrolPointTask.h"
#include"NPCAIController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"Blueprint/AIBlueprintHelperLibrary.h"
#include"BlackboardKey.h"


UPatrolPointTask::UPatrolPointTask(FObjectInitializer const & ObjectInitializer)
{
	NodeName = TEXT("FindPatrolPath");
}

EBTNodeResult::Type UPatrolPointTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	// get aicontroller
	ANPCAIController* const  AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	// Get Current Patrol points index from the blackboard

	auto PPIndex = AIController->GetBlackboard()->GetValueAsInt(BB_KEYS::PatrolPathIndex);

	// use the index to get the current patrolPath from the npc's reference to the patrolPath
	auto const NPC = Cast<ANPC>(AIController->GetPawn());

	FVector PatrolPoint = NPC->GetPatrolPath()->GetPatrolPoint(PPIndex);

	// the patrolpoints position is not the global so we have to transform this from local to global
	FVector const GloablPoint = NPC->GetPatrolPath()->GetActorTransform().TransformPosition(PatrolPoint);

	// write the current global patrolpoint to the blackboard
	AIController->GetBlackboard()->SetValueAsVector(BB_KEYS::PatrolPathVector, GloablPoint);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;
}
