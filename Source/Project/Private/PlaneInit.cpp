#include "PlaneInit.h"
#include <UObject/UnrealTypePrivate.h>

// Sets default values for this component's properties
APlaneInit::APlaneInit()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryActorTick.bCanEverTick = true;

}


// Initiliaze plane actors based on first coordinates and attitude
void APlaneInit::InitPlaneActors() {
	for (auto& currPlane : planes) {
		int32_t planeNum = currPlane.Key;
		TArray<TArray<float>> coord = coordinates[planeNum];
		FVector loc = FVector(coord[1][0], coord[2][0], coord[3][0]);
		FRotator rot = FRotator(coord[5][0], coord[6][0], coord[4][0]); // constructor is pitch, yaw, roll
		FActorSpawnParameters spawnInfo;
		APlaneActor* spawnedPlane = (APlaneActor*) GetWorld()->SpawnActor<APlaneActor>(APlaneActor::StaticClass(), loc, rot, spawnInfo);
		planeActors.Add(planeNum, spawnedPlane);
	}
}



// Called when the game starts
void APlaneInit::BeginPlay()
{
	Super::BeginPlay();

	FString file = FPaths::ProjectContentDir();
	file.Append(TEXT("Data/trajectory.txt"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (FileManager.FileExists(*file)) {
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None)) {
			TextParser parser;
			parser.ParseData(FileContent);
			planes = parser.getPlanes();
			coordinates = parser.getCoordinates();
			// ParseData(FileContent);
			// FillData();
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load text"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load file"));
	}

	InitPlaneActors();
	numPlanes = planeActors.Num();
	maxCount = coordinates[0][0].Num() - 1;
	
}

// Change plane positions and rotations based on counter which is updated in TickComponent
void APlaneInit::updatePlanePositions() {
	for (auto& curr : planeActors) {
		std::int32_t planeNum = curr.Key;
		APlaneActor* actor = curr.Value;
		TArray<TArray<float>>& coord = coordinates[planeNum];

		int32_t countInt = counter;

		FVector loc = FVector(coord[1][countInt], coord[2][countInt], coord[3][countInt]);
		FRotator rot = FRotator(coord[5][countInt], coord[6][countInt], coord[4][countInt]); // constructor is pitch, yaw, roll

		actor->SetActorLocation(loc);
		actor->SetActorRotation(rot);
	}
}

float APlaneInit::getCounter_Implementation() {
	return counter;
}

void APlaneInit::setCounter_Implementation(float num) {
	counter = num;
}

void APlaneInit::setPlay_Implementation(bool cond) {
	play = cond;
}

void APlaneInit::changeMesh_Implementation(const FString& input) {
	for (auto& curr : planeActors) {
		APlaneActor* actor = curr.Value;
		actor->setMesh(input);
	}
}

APlaneActor* APlaneInit::getPlane_Implementation(int index) {
	return planeActors[index];
}


// Called every frame
void APlaneInit::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (play) {
		counter += 1;
		
	}

	if (counter > maxCount) { // loop
		counter = 1;
	}

	updatePlanePositions();

	
}

