// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Camera/LyraCameraMode.h"
#include "Engine/Engine.h"

#include "CHCameraMode_TopDown.generated.h"

class UObject;
class UPlayerController;

/**
 * UCHCameraMode_TopDown
 *
 *	A TopDown camera mode that follows player movement and tracks mouse cursor aim direction.
 *	Based on TwinStickCharacter aiming logic for consistent TopDown gameplay experience.
 */
UCLASS(Abstract, Blueprintable)
class UCHCameraMode_TopDown : public ULyraCameraMode
{
	GENERATED_BODY()

public:

	UCHCameraMode_TopDown();

protected:

	//~ULyraCameraMode interface
	virtual void UpdateView(float DeltaTime) override;
	virtual void OnActivation() override;
	virtual void OnDeactivation() override;

	virtual FVector GetPivotLocation() const override;
	//~End of ULyraCameraMode interface

private:
	/** Height of the camera above the target actor */
	UPROPERTY(EditDefaultsOnly, Category = "TopDown Camera")
	float CameraHeight = 1000.0f;
	
	/** Pitch angle of the camera (negative for looking down) */
	UPROPERTY(EditDefaultsOnly, Category = "TopDown Camera") 
	float CameraPitch = -70.0f;
	
	/** Trace channel to use for mouse aim detection (same as TwinStickCharacter) */
	UPROPERTY(EditDefaultsOnly, Category = "TopDown Camera")
	TEnumAsByte<ETraceTypeQuery> MouseAimTraceChannel;
	
	/** Whether the camera should smoothly follow player movement */
	UPROPERTY(EditDefaultsOnly, Category = "TopDown Camera")
	bool bFollowPlayerMovement = true;
	
	/** Speed at which camera follows the player (higher = faster response) */
	UPROPERTY(EditDefaultsOnly, Category = "TopDown Camera")
	float FollowSpeed = 5.0f;

	TObjectPtr<APlayerController> PlayerController;	
};