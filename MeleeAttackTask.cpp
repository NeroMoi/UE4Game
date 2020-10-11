// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAttackTask.h"
#include "CombatInterface.h"
#include"NPCAIController.h"
#include"Kismet/KismetSystemLibrary.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include"Engine/LatentActionManager.h"
#include"Components/SkeletalMeshComponent.h"
#include"Animation/AnimMontage.h"
#include"Animation/AnimInstance.h"

UMeleeAttackTask::UMeleeAttackTask()
{
	NodeName = TEXT("MeleeAttack");
}
EBTNodeResult::Type UMeleeAttackTask::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ANPCAIController * const AIController = Cast<ANPCAIController>(OwnerComp.GetAIOwner());
	ANPC * const Npc = Cast<ANPC>(AIController->GetPawn());

	if (ICombatInterface * const ICombat = Cast<ICombatInterface>(Npc))
	{
		if (MontageHasFinished(Npc))
		{
			ICombat->Execute_MeleeAttack(Npc);
		}
	}

	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

bool UMeleeAttackTask::MontageHasFinished(ANPC * const Npc)
{
	return Npc->GetMesh()->GetAnimInstance()->Montage_GetIsStopped(Npc->GetMontage());
}
