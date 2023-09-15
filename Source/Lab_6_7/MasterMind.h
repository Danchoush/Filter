// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Burrel.h"
#include "Balls.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MasterMind.generated.h"

UCLASS()
class LAB_6_7_API AMasterMind : public AActor
{
	GENERATED_BODY()

public:
	AMasterMind();

	virtual void Tick(float DeltaSeconds) override;

	void StopSimulation();

protected:
	virtual void BeginPlay() override;

private:
	void BindToInput();

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _radiusExternal;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _heightExternal;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _radiusInternal;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _heightDelta;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		int32 _discretization;

	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _ballsOffsetZ;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _ballRadius;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		float _ballDeltaRadius;

	UPROPERTY()
		ABurrel* _burrel;
	UPROPERTY()
		ABalls* _balls;

	bool _isStopped;
	int SpaceClick = 0;
};
