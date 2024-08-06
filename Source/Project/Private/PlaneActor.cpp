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

}

void APlaneActor::setMesh(FString input) {
	if (input == TEXT("Quadcopter")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Quadcopter/quadcopter.quadcopter"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Biplane")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Biplane/biplane.biplane"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Drone")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/PassengerCarrier/drone.drone"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Hale")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Hale/hale.hale"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Reaper")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Reaper/reaper.reaper"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Global Hawk")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/GlobalHawk/hawk.hawk"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("RQ14")) { 
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/RQ14/rq14.rq14"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Quadrotor")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Quadrotor/quadrotor.quadrotor"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Octocopter")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Octocopter/octocopter.octocopter"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Hexcopter")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Hexcopter/hexcopter.hexcopter"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("3DR Aero M")) { 
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/3DR/3dr.3dr"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Balloon")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Balloon/balloon.balloon"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Bell 206")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Bell/bell.bell"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else if (input == TEXT("Buster Drone")) {
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Buster/buster.buster"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
	else { // Airplane
		PlaneMesh = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Models/Plane/plane.plane"));
		Mesh->SetStaticMesh(PlaneMesh);
		Mesh->SetupAttachment(Root);
	}
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

