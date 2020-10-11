// Fill out your copyright notice in the Description page of Project Settings.


#include "NPCAIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include"BehaviorTree/BehaviorTree.h"
#include"BehaviorTree/BlackboardComponent.h"
#include"UObject/ConstructorHelpers.h"
#include"AITestCharacter.h"
#include"Perception/AISenseConfig_Sight.h"
#include"Perception/AISenseConfig_Hearing.h"
#include"Perception/AIPerceptionStimuliSourceComponent.h"
#include"Perception/AIPerceptionComponent.h"
#include"BlackboardKey.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include"Runtime/Engine/Classes/Engine/World.h"
#include"GameFramework/Character.h"
#include"AITags.h"
#include"GameFramework/Character.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include"Runtime/Engine/Classes/Engine/Engine.h"

ANPCAIController::ANPCAIController(FObjectInitializer const & ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> ObjTree(TEXT("BehaviorTree'/Game/AI/AILogic/NpcBT.NpcBT'"));

	if (ObjTree.Succeeded())
	{
		BTree = ObjTree.Object;
	}

	BTComponent = ObjectInitializer.CreateDefaultSubobject<UBehaviorTreeComponent>(this, TEXT("BehaviorTreeComp"));
	Blackboard = ObjectInitializer.CreateDefaultSubobject<UBlackboardComponent>(this, TEXT("BlackboardComp"));

	SetupPerceptionSystem();
}



void ANPCAIController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(BTree);
	BTComponent->StartTree(*BTree);
}

void ANPCAIController::OnPossess(APawn * const Pawn)
{
	Super::OnPossess(Pawn);

	if (Blackboard)
	{
		Blackboard->InitializeBlackboard(*BTree->BlackboardAsset);
	}
}

UBlackboardComponent * ANPCAIController::GetBlackboard() const
{
	return Blackboard;
}

void ANPCAIController::OnTargetDetected(AActor * actor, FAIStimulus const stimulus)
{
	if (auto const ch = Cast<AAITestCharacter>(actor))
	{
		GetBlackboard()->SetValueAsBool(BB_KEYS::CanSeePlayer, stimulus.WasSuccessfullySensed());
	}
}

void ANPCAIController::OnUpdate(TArray<AActor*> const & UpdateActors)
{
	for (size_t i = 0; i < UpdateActors.Num(); ++i)
	{
		FActorPerceptionBlueprintInfo info;

		//Retrieves whatever has been sensed about given actor
		GetPerceptionComponent()->GetActorsPerception(UpdateActors[i], info);

		for (size_t j = 0; j < info.LastSensedStimuli.Num(); ++j)
		{
			FAIStimulus const stimulus = info.LastSensedStimuli[j];
			if (stimulus.Tag == AITags::NoiseTag)
			{
				GetBlackboard()->SetValueAsBool(BB_KEYS::IsInvestigating, stimulus.WasSuccessfullySensed());
				GetBlackboard()->SetValueAsVector(BB_KEYS::TargetLocation, stimulus.StimulusLocation);
			}
			else if(stimulus.Type.Name == "Default__AISense_Sight")
			//else
			{
				GetBlackboard()->SetValueAsBool(BB_KEYS::CanSeePlayer, stimulus.WasSuccessfullySensed());
			}
		}
	}
}

void ANPCAIController::SetupPerceptionSystem()
{

	//Create and initialize sight configuration object

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	if (SightConfig)
	{
		SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent")));

		SightConfig->SightRadius = 500.0f;
		SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;

		SightConfig->PeripheralVisionAngleDegrees = 120.0f;
		//set to five seconds after the perceived stimulus is forgotten 
		SightConfig->SetMaxAge(5.0f);

		SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
		SightConfig->DetectionByAffiliation.bDetectEnemies = true;
		SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
		SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add sight configuration component to perception component
		//Indicated sense that takes precedence over other senses when determining sensed actor's location.
		GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
		//AI HearingConfig has do that check
	//	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnTargetDetected);
		GetPerceptionComponent()->ConfigureSense(*SightConfig);
	}

	// create and initialize hearing config object
	
	HearingConfig = CreateDefaultSubobject<UAISenseConfig_Hearing>(TEXT("HearingConfig"));
	if (HearingConfig)
	{
		HearingConfig->HearingRange = 3000.0f;

		HearingConfig->DetectionByAffiliation.bDetectEnemies = true;
		HearingConfig->DetectionByAffiliation.bDetectFriendlies = true;
		HearingConfig->DetectionByAffiliation.bDetectNeutrals = true;

		//add sight configuration component to perception component

		
		GetPerceptionComponent()->OnPerceptionUpdated.AddDynamic(this, &ANPCAIController::OnUpdate);
		GetPerceptionComponent()->ConfigureSense(*HearingConfig);

	}



}
