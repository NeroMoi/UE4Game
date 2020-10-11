// Fill out your copyright notice in the Description page of Project Settings.


#include "AICharacterBase.h"
#include"Components/BoxComponent.h"
#include"Engine/Engine.h"


// Sets default values
AAICharacterBase::AAICharacterBase():
	rightFistCollisionBox(CreateDefaultSubobject<UBoxComponent>(TEXT("RightFistCollBox")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (rightFistCollisionBox)
	{
		FVector const extent(8.0f);

		// read it cause any overlap events to happen
		rightFistCollisionBox->SetBoxExtent(extent, false);
		rightFistCollisionBox->SetCollisionProfileName("NoCollision");

	}

}

// Called when the game starts or when spawned
void AAICharacterBase::BeginPlay()
{
	Super::BeginPlay();

	if (rightFistCollisionBox)
	{
		// location,rotation,Scale,simulateBodies
		FAttachmentTransformRules const rules(EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false);

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You light the fire ang gain %d xp."), 1));
		rightFistCollisionBox->AttachToComponent(GetMesh(), rules, "hand_r_socket");
		rightFistCollisionBox->SetRelativeLocation(FVector(-7.0f, 0.0f, 0.0f));

	}
	
}

// Called every frame
void AAICharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AAICharacterBase::AttackStart()
{
	rightFistCollisionBox->SetCollisionProfileName("Fist");
	rightFistCollisionBox->SetNotifyRigidBodyCollision(true);
}

void AAICharacterBase::AttackEnd()
{
	rightFistCollisionBox->SetCollisionProfileName("NoCollision");
	rightFistCollisionBox->SetNotifyRigidBodyCollision(false);
}




