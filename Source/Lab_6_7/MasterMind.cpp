#include "MasterMind.h"

AMasterMind::AMasterMind()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SetRootComponent(
		CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent")));
}

void AMasterMind::BeginPlay()
{
	Super::BeginPlay();

	if (_discretization < 5) return;

	UWorld* world = GetWorld();


	_burrel = world->SpawnActor<ABurrel>(ABurrel::StaticClass());
	_burrel->Construct(_radiusInternal, _radiusExternal, _heightDelta, _heightExternal,
		GetActorLocation(), _discretization);

	_balls = world->SpawnActor<ABalls>(ABalls::StaticClass());

	BindToInput();
}

void AMasterMind::Tick(float DeltaSeconds)
{
	if (SpaceClick == 1) {
		_balls->Construct(_radiusInternal, _radiusExternal, _heightExternal - _heightDelta,_heightExternal, GetActorLocation(), _ballsOffsetZ, _ballRadius, _ballDeltaRadius);
		SpaceClick++;
	}
	if (SpaceClick == 3)
		_balls->StopSimulation();
	if (SpaceClick == 4)
		_burrel->Destroy();
}

void AMasterMind::StopSimulation()
{
	SpaceClick++;
}

void AMasterMind::BindToInput()
{
	InputComponent = NewObject<UInputComponent>(this);
	InputComponent->RegisterComponent();

	InputComponent->BindAction("Stop Simulation", IE_Pressed,
		this, &AMasterMind::StopSimulation);
	EnableInput(GetWorld()->GetFirstPlayerController());
}

