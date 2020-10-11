// Fill out your copyright notice in the Description page of Project Settings.


#include "FindPlayerLocation.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "NPCAIController.h"
#include"BehaviorTree/Blackboard/BlackboardKeyType.h"
#include"BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include"GameFramework/Character.h"
#include"BlackboardKey.h"



UFindPlayerLocation::UFindPlayerLocation(FObjectInitializer const & ObjectInitializer)
{
	NodeName = TEXT("FindPlayerLocation");
}
EBTNodeResult::Type UFindPlayerLocation::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	ACharacter * const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	auto const ControllerAI = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	
	FVector const PlayerLocation = Player->GetActorLocation();


	if (SearchRandom)
	{
		FNavLocation Loc;

		UNavigationSystemV1* const  NavSys = UNavigationSystemV1::GetCurrent(GetWorld());

		if (NavSys->GetRandomPointInNavigableRadius(PlayerLocation, SearchRadius, Loc, nullptr))
		{
			ControllerAI->GetBlackboard()->SetValueAsVector(BB_KEYS::TargetLocation, Loc.Location);
		}
	}
	else
	{
		ControllerAI->GetBlackboard()->SetValueAsVector(BB_KEYS::TargetLocation, PlayerLocation);
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);



	return EBTNodeResult::Succeeded;
}
