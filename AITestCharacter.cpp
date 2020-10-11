// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AITestCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include"Runtime/Engine/Classes/Engine/World.h"
#include"Components/SkeletalMeshComponent.h"
#include"Materials/MaterialInstanceDynamic.h"
#include"GameFramework/PlayerController.h"

#include"Perception/AIPerceptionStimuliSourceComponent.h"
#include"Perception/AISense_Sight.h"
#include"Perception/AISense_Hearing.h"
#include"AITags.h"
#include"Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include"Runtime/Engine/Classes/Engine/Engine.h"
#include "Components/WidgetComponent.h"
#include"UObject/ConstructorHelpers.h"
#include"Blueprint/UserWidget.h"
#include"HealthBar.h"
//#include"Runtime/Engine/Classes/Components/BoxComponent.h"
#include"Components/BoxComponent.h"
#include"NPC.h"


//////////////////////////////////////////////////////////////////////////
// AAITestCharacter

AAITestCharacter::AAITestCharacter():health(maxHealth), 
									 widgetComponent(CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthValue")))
									 
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)


	SetupStimulus();

	if (widgetComponent)
	{
		widgetComponent->SetupAttachment(RootComponent);
		widgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
		widgetComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 100.0f));
		static ConstructorHelpers::FClassFinder<UUserWidget> widgetClass(TEXT("/Game/UI/HealthBar_BP"));

		if (widgetClass.Succeeded())
		{
			widgetComponent->SetWidgetClass(widgetClass.Class);
		}
	}

	
}

//////////////////////////////////////////////////////////////////////////
// Input

void AAITestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AAITestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AAITestCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AAITestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AAITestCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AAITestCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AAITestCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &AAITestCharacter::OnResetVR);

	// exit game functionally
	PlayerInputComponent->BindAction("ExitGame", IE_Pressed, this, &AAITestCharacter::ExitGame);

	PlayerInputComponent->BindAction("PunchAttack", IE_Pressed, this, &AAITestCharacter::OnPunchAttack);

	PlayerInputComponent->BindAction("Distract", IE_Pressed, this, &AAITestCharacter::OnDistract);


}

void AAITestCharacter::ExitGame()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}

void AAITestCharacter::SetupStimulus()
{
	Stimulus = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus"));
	Stimulus->RegisterForSense(TSubclassOf<UAISense_Sight>());
	Stimulus->RegisterWithPerceptionSystem();
}

void AAITestCharacter::OnPunchAttack()
{
	if (Montage)
	{
		PlayAnimMontage(Montage);
	}
}

void AAITestCharacter::OnDistract()
{
	if (DistractionSound)
	{
		FVector const location = GetActorLocation();
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DistractionSound, location);
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), location, 1.0f, this, 0.0f, AITags::NoiseTag);
	}
}

void AAITestCharacter::OnAttackOverlapBegin(UPrimitiveComponent * const overlapped_component, AActor * const other_actor, UPrimitiveComponent * other_component, int32 other_body_index, bool const from_sweep, FHitResult const & sweep_result)
{
	if (ANPC* const npc = Cast<ANPC>(other_actor))
	{
		float const new_health = npc->GetHealth() - npc->GetMaxHealth() * 0.1f;
		npc->SetHealth(new_health);
	}
}

void AAITestCharacter::OnAttackOverlapEnd(UPrimitiveComponent * const overlapped_component, AActor * const other_actor, UPrimitiveComponent * other_component, int32 other_body_index)
{

}


void AAITestCharacter::BeginPlay()
{
	Super::BeginPlay();

	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("You light the fire ang gain %d xp."), 2));
	/*limit pitch of camera*/
	APlayerCameraManager * const CamManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	CamManager->ViewPitchMax = 60.0f;
	CamManager->ViewPitchMin = -10.0f;

	/* set material color of character*/
	UMaterialInstanceDynamic * const MaterialInstance = UMaterialInstanceDynamic::Create(GetMesh()->GetMaterial(0), this);
	if (MaterialInstance)
	{
		MaterialInstance->SetVectorParameterValue("BodyColor", FLinearColor(1.0f, 0.0f, 0.0f, 1.0f));
		GetMesh()->SetMaterial(0, MaterialInstance);
	}

	
	if (rightFistCollisionBox)
	{
		rightFistCollisionBox->SetGenerateOverlapEvents(true);
		rightFistCollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AAITestCharacter::OnAttackOverlapBegin);
		rightFistCollisionBox->OnComponentEndOverlap.AddDynamic(this, &AAITestCharacter::OnAttackOverlapEnd);



	}


}

void AAITestCharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void AAITestCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AAITestCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

float AAITestCharacter::GetHealth() const
{
	return health;
}

float AAITestCharacter::GetMaxHealth() const
{
	return maxHealth;
}

void AAITestCharacter::SetHealth(float const newHealth)
{
	health = newHealth;

	if (health <= 0)
	{
		UE_LOG(LogTemp, Error, TEXT("you lose !!"));
		auto const Controller = UGameplayStatics::GetPlayerController(this, 0);
		Controller->SetCinematicMode(true, false, false, true, true);
		GetMesh()->SetSimulatePhysics(true);
		GetMovementComponent()->MovementState.bCanJump = false;
		GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}

void AAITestCharacter::Tick(float const deltaTime)
{
	Super::Tick(deltaTime);

	auto const uWidget = Cast<UHealthBar>(widgetComponent->GetUserWidgetObject());

	if (uWidget)
	{
		uWidget->SetBarValuePercent(health / maxHealth);
		
	}

}

void AAITestCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAITestCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AAITestCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AAITestCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
