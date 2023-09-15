// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Balls.generated.h"

UCLASS()
class LAB_6_7_API ABalls : public AActor
{
	GENERATED_BODY()

public:
	ABalls();
	void Construct(float r, float R, float h, float H, FVector position,
		float offsetZ, float ballR, float ballDeltaR);
	void StopSimulation();
	void SetParameters(float radius);

private:
	UPROPERTY()
		UStaticMesh* _mesh;
	UPROPERTY()
		UMaterialInstanceDynamic* BallActorMaterial;
	UPROPERTY()
		TArray<ABalls*> _balls;

	float _radiusExternal;
	float _heightExternal;
	float _radiusInternal;
	float _heightInternal;
	FVector _position;
};
