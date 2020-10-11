// Fill out your copyright notice in the Description page of Project Settings.


#include "ChasePlayer.h"
#include"NPCAIController.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"Blueprint/AIBlueprintHelperLibrary.h"
#include"BlackboardKey.h"


UChasePlayer::UChasePlayer(FObjectInitializer const& ObjctInitializer)
{
	NodeName = TEXT("ChasePlayer");
}
EBTNodeResult::Type UChasePlayer::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8* NodeMemory)
{
	// get TargetLocation from blackboard via ai controller

	ANPCAIController * const AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	FVector const PlayerLocation = AIController->GetBlackboard()->GetValueAsVector(BB_KEYS::TargetLocation);

	// move to player location
	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, PlayerLocation);

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

	return EBTNodeResult::Succeeded;


}

