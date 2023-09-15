// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProceduralMeshComponent.h"
#include "Burrel.generated.h"

UCLASS()
class LAB_6_7_API ABurrel : public AActor
{
	GENERATED_BODY()

public:
	ABurrel();
	void Construct(float r, float R, float h, float H, FVector position, int32 discretization);

	UPROPERTY()
		UProceduralMeshComponent* _proceduralMesh;

private:
	
	void ConstructFloor(int32 index, float radius, int32 discretization, bool isInverseFace,
		FVector offset = FVector());
	void ConstructWall(int32 index, float radius, float height, int32 discretization, bool isInverseFace,
		FVector offset = FVector());
};