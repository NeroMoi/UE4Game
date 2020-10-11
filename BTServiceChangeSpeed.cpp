// Fill out your copyright notice in the Description page of Project Settings.


#include "BTServiceChangeSpeed.h"
#include"NPC.h"
#include"NPCAIController.h"
#include"GameFramework/CharacterMovementComponent.h"

UBTServiceChangeSpeed::UBTServiceChangeSpeed()
{
	bNotifyBecomeRelevant = true;
	NodeName = TEXT("Change AI Speed");
}
void UBTServiceChangeSpeed::OnBecomeRelevant(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	Super::OnBecomeRelevant(OwnerComp, NodeMemory);

	ANPCAIController* const  AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());

	auto const NPC = Cast<ANPC>(AIController->GetPawn());

	NPC->GetCharacterMovement()->MaxWalkSpeed = AISpeed;
}

FString UBTServiceChangeSpeed::GetStaticServiceDescription() const
{
	return FString("Change the NPC's Speed");
}
