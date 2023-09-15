#include "Balls.h"

ABalls::ABalls()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	UStaticMeshComponent* sphereComponent =
		CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	sphereComponent->AttachToComponent(GetRootComponent(),
		FAttachmentTransformRules(EAttachmentRule::SnapToTarget, false));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> s_SphereMesh(
		TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	sphereComponent->SetStaticMesh(s_SphereMesh.Object);
	sphereComponent->SetCastShadow(false);
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> ActorMaterial(
		TEXT("Material'/Game/StarterContent/Materials/M_Water_Lake.M_Water_Lake'"));
	BallActorMaterial = UMaterialInstanceDynamic::Create(ActorMaterial.Object, nullptr);

	sphereComponent->SetMaterial(0, BallActorMaterial);
	sphereComponent->SetSimulatePhysics(true);
}

void ABalls::Construct(float r, float R, float h, float H, FVector position,
	float offsetZ, float ballR, float ballDeltaR)
{
	UWorld* world = GetWorld();

	_radiusExternal = R;
	_radiusInternal = r;
	_heightExternal = H;
	_heightInternal = h;
	_position = position;

	const float externalVolume = PI * R * R * H;
	const float internalVolume = PI * r * r * h;
	const float targetVolume = externalVolume - internalVolume;

	_balls.Reserve(targetVolume / 4.0 / 3.0 * PI * ballR * ballR * ballR);

	float filledVolume = 0;
	float zPosition = H + offsetZ;
	const float radiusSpawnMin = r + ballR + ballDeltaR;
	const float radiusSpawnMax = R - ballR - ballDeltaR;
	float radiusSpawn = radiusSpawnMin;
	const float addingRadiusSpawn = (ballR + ballDeltaR) * 2 + 10;
	float angle = 0;
	float addingAngle = 2 * FMath::Asin((ballR + ballDeltaR + 10) / radiusSpawn);

	while (filledVolume < targetVolume)
	{
		if (angle > 2 * PI)
		{
			radiusSpawn += addingRadiusSpawn;

			if (radiusSpawn > radiusSpawnMax)
			{
				radiusSpawn = radiusSpawnMin;
				zPosition += 2 * (ballR + ballDeltaR) + 10;
			}

			angle = 0;
			addingAngle = 2 * FMath::Asin((ballR + ballDeltaR + 10) / radiusSpawn);
		}

		const float randomBallR = FMath::RandRange(ballR - ballDeltaR, ballR + ballDeltaR);

		ABalls* ball = world->SpawnActor<ABalls>(
			position + FVector(radiusSpawn * FMath::Cos(angle),
				radiusSpawn * FMath::Sin(angle), zPosition), FRotator());

		ball->SetParameters(randomBallR);
		_balls.Add(ball);

		filledVolume += 4.0 / 3.0 * PI * randomBallR * randomBallR * randomBallR;
		angle += addingAngle;
	}
}

void ABalls::SetParameters(float radius)
{
	float const scale = radius / 100 * 2;
	SetActorScale3D(FVector(scale, scale, scale));
}

void ABalls::StopSimulation()
{
	float maxZ = _position.Z + _heightExternal;

	for (ABalls* ball : _balls)
	{
		FVector ballLocation = ball->GetActorLocation();

		if (ballLocation.Z > maxZ
			|| ballLocation.Z < _position.Z
			|| FMath::Pow(ballLocation.X - _position.X, 2)
			+ FMath::Pow(ballLocation.Y - _position.Y, 2) > FMath::Pow(_radiusExternal, 2))
		{
			ball->Destroy();
		}
		else
		{
			ball->DisableComponentsSimulatePhysics();
		}
	}
}

