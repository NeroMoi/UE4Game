// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "AITestGameMode.h"
#include "AITestCharacter.h"
#include "UObject/ConstructorHelpers.h"

AAITestGameMode::AAITestGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
