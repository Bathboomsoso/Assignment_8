#include "SpartaGameMode.h"
#include "SpartaCharacter.h"
#include "SpartaPlayerController.h"
#include "SpartaPlayerController.h"
#include "SpartaGameState.h"

void ASpartaGameMode::ASpartaGamemode()
{
	DefaultPawnClass = ASpartaCharacter::StaticClass();
	PlayerControllerClass = ASpartaPlayerController::StaticClass();
	GameStateClass = ASpartaGameState::StaticClass();
}
