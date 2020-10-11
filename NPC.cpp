// Fill out your copyright notice in the Description page of Project Settings.


#include "NPC.h"

#include"Runtime/Engine/Classes/Engine/World.h"
#include"Components/SkeletalMeshComponent.h"
#include"Materials/MaterialInstanceDynamic.h"
#include"GameFramework/PlayerController.h"
#include"GameFramework/CharacterMovementComponent.h"

#include "Components/WidgetComponent.h"
#include"UObject/ConstructorHelpers.h"
#include"Blueprint/UserWidget.h"
#include"HealthBar.h"
#include"AITestCharacter.h"
#include"Components/BoxComponent.h"

// Sets default values
ANPC::ANPC():health(maxHealth),
			widgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	bUseControllerRotationYaw = false;
	//GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = true;

	if (widgetComponent)
	{
		widgetComponent->SetupAttachment(RootComponent);
		widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 85.0f));
		static ConstructorHelpers::FClassFinder<UUserWidget> widgetClass(TEXT("/Game/UI/HealthBar_BP"));

		if (widgetClass.Succeeded())
		{
			widgetComponent->SetWidgetClass(widgetClass.Class);
		}
	}

}

// Called when the game starts or when spawned
void ANPC::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInstanceDynamic * const MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("BodyColor", FLinearColor(0.0f, 1.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, MaterialInstance);
	}
	

	if (rightFistCollisionBox)
	{
		rightFistCollisionBox->SetGenerateOverlapEvents(true);
		rightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ANPC::AttackOverlapBegin);
		rightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &ANPC::AttackOverlapEnd);
	}
}

void ANPC::AttackOverlapBegin(UPrimitiveComponent * const overlapped_component, AActor * const other_actor, UPrimitiveComponent * other_component, int32 other_body_index, bool const from_sweep, FHitResult const & sweep_result)
{
	if (AAITestCharacter* const player = Cast<AAITestCharacter>(other_actor))
	{
		float const new_health = player->GetHealth() - player->GetMaxHealth() * 0.07f;
		player->SetHealth(new_health);
	}
}

void ANPC::AttackOverlapEnd(UPrimitiveComponent * const overlapped_component, AActor * const other_actor, UPrimitiveComponent * other_component, int32 other_body_index)
{

}

// Called every frame
void ANPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto const uWidget = Cast<UHealthBar>(widgetComponent->GetUserWidgetObject());

	if (uWidget)
	{
		uWidget->SetBarValuePercent(health / maxHealth);
	}

}

// Called to bind functionality to input
void ANPC::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

APatrolPath * ANPC::GetPatrolPath()
{
	return PatrolPath;
}

int ANPC::MeleeAttack_Implementation()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
	return 0;
}

UAnimMontage * ANPC::GetMontage() const
{
	return Montage;
}

float ANPC::GetHealth() const
{
	return health;
}

float ANPC::GetMaxHealth() const
{
	return maxHealth;
}

void ANPC::SetHealth(float const newHealth)
{
	health = newHealth;


}

