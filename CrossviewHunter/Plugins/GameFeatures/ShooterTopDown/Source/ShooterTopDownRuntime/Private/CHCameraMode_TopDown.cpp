// Copyright Epic Games, Inc. All Rights Reserved.

#include "CHCameraMode_TopDown.h"

#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "Character/LyraCharacter.h"
#include "GameplayTagContainer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CHCameraMode_TopDown)

UCHCameraMode_TopDown::UCHCameraMode_TopDown()
{
	CameraHeight = 1000.0f;
	CameraPitch = -66.0f;
	bFollowPlayerMovement = true;
	FollowSpeed = 5.0f;
	
	// Set default FOV for TopDown view
	FieldOfView = 90.0f;
	
	// Set default trace channel (can be customized in Blueprint)
	MouseAimTraceChannel = UEngineTypes::ConvertToTraceType(ECC_WorldStatic);
}

void UCHCameraMode_TopDown::UpdateView(float DeltaTime)
{
	FVector PivotLocation = GetPivotLocation();
	FRotator PivotRotation = FRotator(CameraPitch, 0.0f, 0.0f);
	FRotator TargetRotation;

	if (!PlayerController)
	{
		AActor* TargetActor = GetTargetActor();
		APawn* TargetPawn = Cast<APawn>(TargetActor);
		AController* TargetController = TargetPawn ? TargetPawn->GetController() : nullptr;
		PlayerController = Cast<APlayerController>(TargetController);
	}
	
	if (PlayerController)
	{
		const FRotator DefaultRotation = PlayerController->GetControlRotation();

		FHitResult OutHit;
		bool bValidMouseHit = PlayerController->GetHitResultUnderCursorByChannel(MouseAimTraceChannel, true, OutHit);
		
		if (bValidMouseHit)
		{
			const FRotator AimRotation = UKismetMathLibrary::FindLookAtRotation(PivotLocation, OutHit.Location);
			
			TargetRotation = FRotator(0.0f, AimRotation.Yaw, 0.0f);


			FCollisionShape SphereShape = FCollisionShape::MakeSphere(5.f);
			UWorld* World = GetWorld();
			DrawDebugSphere(World, FVector(OutHit.Location.X, OutHit.Location.Y, PivotLocation.Z), SphereShape.Sphere.Radius, 8, FColor::Red);
			DrawDebugLine(World, PivotLocation, FVector(OutHit.Location.X, OutHit.Location.Y, PivotLocation.Z), FColor::Red);
		}
		else
		{
			TargetRotation = FRotator(0.0f, DefaultRotation.Yaw, 0.0f);
		}
	}
	else
	{
		// Default TopDown rotation when no player controller
		TargetRotation = FRotator(0.0f, 0.0f, 0.0f);
	}

	// Set the view data
	View.Location = PivotLocation - PivotRotation.Vector() * CameraHeight;
	View.Rotation = PivotRotation;
	View.ControlRotation = TargetRotation;
	View.FieldOfView = FieldOfView;
}

void UCHCameraMode_TopDown::OnActivation()
{
	Super::OnActivation();
	AActor* TargetActor = GetTargetActor();
	ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(TargetActor);
	if (GetCameraTypeTag().IsValid() && LyraCharacter != nullptr)
	{
		if (UAbilitySystemComponent* ASC = LyraCharacter->GetAbilitySystemComponent())
		{
			ASC->AddLooseGameplayTag(GetCameraTypeTag());
		}
	}
}

void UCHCameraMode_TopDown::OnDeactivation()
{
	Super::OnDeactivation();
	ALyraCharacter* LyraCharacter = Cast<ALyraCharacter>(PlayerController->GetCharacter());
	if (GetCameraTypeTag().IsValid() && LyraCharacter != nullptr)
	{
		if (UAbilitySystemComponent* ASC = LyraCharacter->GetAbilitySystemComponent())
		{
			ASC->RemoveLooseGameplayTag(GetCameraTypeTag());
		}
	}
}

FVector UCHCameraMode_TopDown::GetPivotLocation() const
{
	const AActor* TargetActor = GetTargetActor();
	check(TargetActor);

	return TargetActor->GetActorLocation();
}
