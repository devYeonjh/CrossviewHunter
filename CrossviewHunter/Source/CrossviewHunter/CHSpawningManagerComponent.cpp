// Fill out your copyright notice in the Description page of Project Settings.


#include "CHSpawningManagerComponent.h"

#include "Engine/World.h"
#include "GameFramework/PlayerState.h"
#include "GameModes/LyraGameState.h"
#include "Player/LyraPlayerStart.h"
#include "AIController.h"
#include "Teams/LyraTeamSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CHSpawningManagerComponent)

class AActor;

UCHSpawningManagerComponent::UCHSpawningManagerComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

AActor* UCHSpawningManagerComponent::OnChoosePlayerStart(AController* Player, TArray<ALyraPlayerStart*>& PlayerStarts)
{
	ULyraTeamSubsystem* TeamSubsystem = GetWorld()->GetSubsystem<ULyraTeamSubsystem>();
	if (!ensure(TeamSubsystem))
	{
		return nullptr;
	}

	const int32 PlayerTeamId = TeamSubsystem->FindTeamFromObject(Player);

	// We should have a TeamId by now, but early login stuff before post login can try to do stuff, ignore it.
	if (!ensure(PlayerTeamId != INDEX_NONE))
	{
		return nullptr;
	}

	ALyraGameState* GameState = GetGameStateChecked<ALyraGameState>();

	ALyraPlayerStart* BestPlayerStart = nullptr;
	double MaxDistance = 0;
	ALyraPlayerStart* FallbackPlayerStart = nullptr;
	double FallbackMaxDistance = 0;

	for (APlayerState* PS : GameState->PlayerArray)
	{
		const int32 TeamId = TeamSubsystem->FindTeamFromObject(PS);
		
		// We should have a TeamId by now...
		if (PS->IsOnlyASpectator() || !ensure(TeamId != INDEX_NONE))
		{
			continue;
		}

		// If the other player isn't on the same team, lets find the furthest spawn from them.

			for (ALyraPlayerStart* PlayerStart : PlayerStarts)
			{
				FGameplayTagContainer PlayerStartTags = PlayerStart->GetGameplayTags();
				if (!PlayerStartTags.IsEmpty())
				{
					if (PlayerTeamId == 1)
					{
						return PlayerStart;
					}
					continue;
				}
				if (TeamId != PlayerTeamId)
				{
					if (APawn* Pawn = PS->GetPawn())
					{
						const double Distance = PlayerStart->GetDistanceTo(Pawn);
					
						if (PlayerStart->IsClaimed())
						{
							if (FallbackPlayerStart == nullptr || Distance > FallbackMaxDistance)
							{
								FallbackPlayerStart = PlayerStart;
								FallbackMaxDistance = Distance;
							}
						}
						else if (PlayerStart->GetLocationOccupancy(Player) < ELyraPlayerStartLocationOccupancy::Full)
						{
							if (BestPlayerStart == nullptr || Distance > MaxDistance)
							{
								BestPlayerStart = PlayerStart;
								MaxDistance = Distance;
							}
						}
					}
				}
			}

	}

	if (BestPlayerStart)
	{
		return BestPlayerStart;
	}

	return FallbackPlayerStart;
}

void UCHSpawningManagerComponent::OnFinishRestartPlayer(AController* Player, const FRotator& StartRotation)
{
	
}
