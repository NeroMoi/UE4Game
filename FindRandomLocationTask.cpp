// Fill out your copyright notice in the Description page of Project Settings.


#include "FindRandomLocationTask.h"
#include "BehaviorTree/BlackboardComponent.h"
#include"Runtime/NavigationSystem/Public/NavigationSystem.h"
#include"NPCAIController.h"
#include"BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include"BlackboardKey.h"


UFindRandomLocationTask::UFindRandomLocationTask(FObjectInitializer const & ObjectInitializer)
{
	NodeName = TEXT("FindRandomLocation");
}
EBTNodeResult::Type UFindRandomLocationTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	auto const ControllerAI = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	auto const NPC = ControllerAI->GetPawn();

	FVector const Origin = NPC->GetActorLocation();
	FNavLocation  NavLoc;

	// get the navigation system and generate a random location on the navmesh
	UNavigationSystemV1 * const NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

	if (NavSys->GetRandomPointInNavigableRadius(Origin, SearchRadius, NavLoc, nullptr))
	{
		ControllerAI->GetBlackboard()->SetValueAsVector(BB_KEYS::TargetLocation, NavLoc.Location);
	}
	
	//Finish with success
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);



	return EBTNodeResult::Succeeded;
}
