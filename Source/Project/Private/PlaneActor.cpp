#include "PlaneActor.h"

// Sets default values
APlaneActor::APlaneActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Plane/plane.plane")); 

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetStaticMesh(PlaneMesh);
	Mesh->SetupAttachment(Root);

	// need to add CesiumGlobeAnchorComponent


}

// Called when the game starts or when spawned
void APlaneActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaneActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

