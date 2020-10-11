// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include"PatrolPath.h"
#include"CombatInterface.h"
#include"Animation/AnimMontage.h"
#include "AICharacterBase.h"
#include "NPC.generated.h"

UCLASS()
class AITEST_API ANPC : public AAICharacterBase,public ICombatInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APatrolPath * GetPatrolPath();

	
	int MeleeAttack_Implementation() override;

	UAnimMontage * GetMontage() const;

	float GetHealth() const;
	float GetMaxHealth() const;
	void SetHealth(float const newHealth);


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	class UWidgetComponent *widgetComponent;

	float const maxHealth = 100.0f;
	float health;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
		APatrolPath * PatrolPath;

	UPROPERTY(EditAnywhere, BluePrintReadWrite, Category = "Animation", meta = (AllowPrivateAccess = "true"))
		UAnimMontage * Montage;

	UFUNCTION()
		void AttackOverlapBegin(
			class UPrimitiveComponent* const overlapped_component,
			class AActor* const other_actor,
			class UPrimitiveComponent* other_component,
			int32  other_body_index,
			bool const from_sweep,
			FHitResult const& sweep_result);

	UFUNCTION()
		void AttackOverlapEnd(
			class UPrimitiveComponent* const overlapped_component,
			class AActor* const other_actor,
			class UPrimitiveComponent* other_component,
			int32  other_body_index);

};

