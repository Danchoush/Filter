// Fill out your copyright notice in the Description page of Project Settings.


#include "Burrel.h"

// Sets default values
ABurrel::ABurrel()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	_proceduralMesh =
		CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProceduralMesh"));
	_proceduralMesh->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));
	_proceduralMesh->bUseAsyncCooking = true;
	_proceduralMesh->SetCastShadow(false);
}

void ABurrel::Construct(float r, float R, float h, float H, FVector position, int32 discretization)
{
	SetActorLocation(position);

	ConstructFloor(0, R, discretization, false, FVector(0, 0, 0));
	ConstructFloor(1, r, discretization, true, FVector(0, 0, h));
	ConstructFloor(2, r, discretization, false, FVector(0, 0, H));

	ConstructWall(3, R, H, discretization, true, FVector(0, 0, 0));
	ConstructWall(4, r, H - h, discretization, false, FVector(0, 0, h));
}

void ABurrel::ConstructFloor(int32 index, float radius, int32 discretization, bool isInverseFace, FVector offset)
{
	TArray<FVector> vertices;
	vertices.Reserve(discretization + 1);

	float angle = 0;
	float const deltaPhi = FMath::DegreesToRadians(360.0 / discretization);

	for (int i = 0; i < discretization + 1; ++i)
	{
		vertices.Add(FVector(radius * FMath::Cos(angle), radius * FMath::Sin(angle), 0) + offset);
		angle += deltaPhi;
	}

	TArray<int32> triangles;
	triangles.Reserve((discretization - 2) * 3);

	if (isInverseFace)
	{
		for (int i = 0; i < discretization - 1; ++i)
		{
			triangles.Add(0);
			triangles.Add(i + 1);
			triangles.Add(i + 2);
		}
	}
	else
	{
		for (int i = 0; i < discretization - 1; ++i)
		{
			triangles.Add(i + 2);
			triangles.Add(i + 1);
			triangles.Add(0);
		}
	}

	TArray<FVector> normals;
	normals.Init(FVector(0, 0, (isInverseFace ? -1 : 1)), discretization + 1);

	TArray<FVector2D> uvs;
	uvs.Reserve(discretization + 1);

	angle = 0;
	for (int i = 0; i < discretization + 1; ++i)
	{
		uvs.Add(FVector2D(0.5 + FMath::Cos(angle) / 2, 0.5 + FMath::Sin(angle) / 2));
		angle += deltaPhi;
	}

	TArray<FLinearColor> colors;
	colors.Init(FLinearColor(0.8, 0.8, 0.8), discretization + 1);

	TArray<FProcMeshTangent> tangents;
	tangents.Reserve(discretization + 1);

	angle = 0;
	for (int i = 0; i < discretization + 1; ++i)
	{
		const FVector normal = FVector(FMath::Cos(angle), FMath::Sin(angle), 0).GetSafeNormal();
		tangents.Add(FProcMeshTangent(normal.X, normal.Y, normal.Z));
		angle += deltaPhi;
	}

	_proceduralMesh->CreateMeshSection_LinearColor(
		index, vertices, triangles, normals, uvs, colors, tangents, true);
}

void ABurrel::ConstructWall(
	int32 index, float radius, float height, int32 discretization, bool isInverseFace, FVector offset)
{
	TArray<FVector> vertices;
	vertices.Reserve(discretization * 2 + 2);

	float angle = 0;
	float const deltaPhi = FMath::DegreesToRadians(360.0 / discretization);

	for (int i = 0; i < discretization + 1; ++i)
	{
		vertices.Add(FVector(radius * FMath::Cos(angle), radius * FMath::Sin(angle), 0) + offset);
		angle += deltaPhi;
	}
	for (int i = 0; i < discretization + 1; ++i)
	{
		vertices.Add(FVector(radius * FMath::Cos(angle), radius * FMath::Sin(angle), height) + offset);
		angle += deltaPhi;
	}

	TArray<int32> triangles;
	triangles.Reserve(discretization * 2 * 3);

	if (isInverseFace)
	{
		for (int i = 0; i < discretization + 1; ++i)
		{
			triangles.Add((i + 1) % (discretization + 1));
			triangles.Add(i + (discretization + 1));
			triangles.Add(i);

			triangles.Add(i + (discretization + 1));
			triangles.Add((i + 1) % (discretization + 1));
			triangles.Add((i + 1) % (discretization + 1) + (discretization + 1));
		}
	}
	else
	{
		for (int i = 0; i < discretization + 1; ++i)
		{
			triangles.Add(i);
			triangles.Add(i + (discretization + 1));
			triangles.Add((i + 1) % (discretization + 1));

			triangles.Add((i + 1) % (discretization + 1) + (discretization + 1));
			triangles.Add((i + 1) % (discretization + 1));
			triangles.Add(i + (discretization + 1));
		}
	}

	TArray<FVector> normals;
	normals.Reserve(discretization * 2 + 2);

	angle = 0;

	if (isInverseFace)
	{
		for (int i = 0; i < discretization + 1; ++i)
		{
			normals.Add(FVector(FMath::Cos(angle), FMath::Sin(angle), 0).GetSafeNormal());
			angle += deltaPhi;
		}
	}
	else
	{
		for (int i = 0; i < discretization + 1; ++i)
		{
			normals.Add(FVector(-FMath::Cos(angle), -FMath::Sin(angle), 0).GetSafeNormal());
			angle += deltaPhi;
		}
	}

	TArray<FVector2D> uvs;
	uvs.Reserve(discretization * 2 + 2);

	for (int i = 0; i < discretization + 1; ++i)
	{
		uvs.Add(FVector2D(i / (discretization + 1), 0));
	}
	for (int i = 0; i < discretization + 1; ++i)
	{
		uvs.Add(FVector2D(i / (discretization + 1), 1));
	}

	TArray<FLinearColor> colors;
	colors.Init(FLinearColor(0.8, 0.8, 0.8), discretization * 2 + 2);

	TArray<FProcMeshTangent> tangents;
	tangents.Reserve(discretization * 2 + 2);

	for (int j = 0; j < 2; ++j)
	{
		angle = 0;
		for (int i = 0; i < discretization + 1; ++i)
		{
			const FVector normal = FVector(FMath::Cos(angle), FMath::Sin(angle), 0).GetSafeNormal();
			tangents.Add(FProcMeshTangent(normal.X, normal.Y, normal.Z));
			angle += deltaPhi;
		}
	}

	_proceduralMesh->CreateMeshSection_LinearColor(
		index, vertices, triangles, normals, uvs, colors, tangents, true);
}


