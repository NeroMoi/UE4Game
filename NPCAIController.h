// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include"Perception/AIPerceptionTypes.h"
#include "NPCAIController.generated.h"

/**
 * 
 */
UCLASS()
class AITEST_API ANPCAIController : public AAIController
{
	GENERATED_BODY()

public:

	ANPCAIController(FObjectInitializer const & ObjectInitializer = FObjectInitializer::Get());
	
	void BeginPlay() override;

	void OnPossess(APawn * const Pawn) override;

	class UBlackboardComponent * GetBlackboard() const;

private:

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTreeComponent*  BTComponent;
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		class UBehaviorTree * BTree;

	class UBlackboardComponent * Blackboard;

	class UAISenseConfig_Sight * SightConfig;

	class UAISenseConfig_Hearing* HearingConfig;

	UFUNCTION()
		void OnTargetDetected(AActor * actor, FAIStimulus const stimulus);

	UFUNCTION()
		void OnUpdate(TArray<AActor*> const& UpdateActors);

	void SetupPerceptionSystem();

};

