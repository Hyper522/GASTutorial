// Copyright Epic Games, Inc. All Rights Reserved.

#include "GASIntroGameMode.h"
#include "GASIntroCharacter.h"
#include "UObject/ConstructorHelpers.h"

AGASIntroGameMode::AGASIntroGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
