// Copyright Epic Games, Inc. All Rights Reserved.

#include "MyInvenGameMode.h"
#include "MyInvenCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMyInvenGameMode::AMyInvenGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
