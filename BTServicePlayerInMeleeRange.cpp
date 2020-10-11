// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServicePlayerInMeleeRange.h"
#include "NPCAIController.h"
#include"NPC.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"BlackboardKey.h"


UBTServicePlayerInMeleeRange::UBTServicePlayerInMeleeRange()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("IsPlayerInMeleeRange");
}
void UBTServicePlayerInMeleeRange::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	//get npc
	ANPCAIController * const AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	ANPC *const Npc = Cast<ANPC>(AIController->GetPawn());

	//get the playerCharacter
	ACharacter * const Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	//write true or false depengding on whether the palyer is within meleeRange

	AIController->GetBlackboard()->SetValueAsBool(BB_KEYS::PlayerInMeleeRange, Npc->GetDistanceTo(Player) <= MeleeRange);
}
