#include "PlaneInit.h"

// Sets default values for this component's properties
UPlaneInit::UPlaneInit()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlaneInit::ParseData(FString &input) {
	// start from second line to create mapping of plane number to plane id
	// loop through the rest of the lines which are time in seconds, plane number, x, y, z, attitude

	TArray<FString> lines;
	input.ParseIntoArray(lines, TEXT("\n"), false);
	for (std::int32_t i = 1; i < lines.Num(); i++) {
		TArray<FString> words;
		lines[i].ParseIntoArray(words, TEXT(","), false);

		if (words[1].Contains(TEXT("fid"))) {
			// add to mapping of planes
			// line should have format "plane number, plane id, plane type"
			planes.Add(FCString::Atoi(*words[0]), words[1]);

			// also initialize map for coordinates
			TArray<float> seconds;
			TArray<float> x;
			TArray<float> y;
			TArray<float> z;
			TArray<float> roll;
			TArray<float> pitch;
			TArray<float> yaw;
			TArray<TArray<float>> coord = { seconds, x, y, z, roll, pitch, yaw };
			coordinates.Add(FCString::Atoi(*words[0]), coord);
		}
		else {
			// add to mapping of coordinates
			// line should have format "time, plane number, x, y, z, attitude"
			float seconds = FCString::Atof(*words[0]);
			int32_t planeNum = FCString::Atoi(*words[1]);
			float x = FCString::Atof(*words[2]);
			float y = FCString::Atof(*words[3]);
			float z = FCString::Atof(*words[4]);

			FString attitude = words[5];
			float roll = FCString::Atof(*(attitude.Mid(0, 2)));
			float pitch = FCString::Atof(*(attitude.Mid(2, 2)));
			float yaw = FCString::Atof(*(attitude.Mid(4, 2)));

			TArray<TArray<float>> &coord = coordinates[planeNum];
			coord[0].Add(seconds);
			coord[1].Add(x);
			coord[2].Add(y);
			coord[3].Add(z);
			coord[4].Add(roll);
			coord[5].Add(pitch);
			coord[6].Add(yaw);
		}

	}
}

// Initiliaze plane actors based on first coordinates and attitude
void UPlaneInit::InitPlaneActors() {
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
void UPlaneInit::BeginPlay()
{
	Super::BeginPlay();

	FString file = FPaths::ProjectContentDir();
	file.Append(TEXT("Data/trajectory.txt"));
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	FString FileContent;
	if (FileManager.FileExists(*file)) {
		if (FFileHelper::LoadFileToString(FileContent, *file, FFileHelper::EHashOptions::None)) {
			ParseData(FileContent);
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load text"));
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Failed to load file"));
	}

	InitPlaneActors();
	
}

// Change plane positions and rotations based on counter which is updated in TickComponent
void UPlaneInit::updatePlanePositions() {
	for (auto& curr : planeActors) {
		std::int32_t planeNum = curr.Key;
		APlaneActor* actor = curr.Value;
		TArray<TArray<float>>& coord = coordinates[planeNum];

		int32_t countInt = counter / updateNum;

		FVector loc = FVector(coord[1][countInt], coord[2][countInt], coord[3][countInt]);
		FRotator rot = FRotator(coord[5][countInt], coord[6][countInt], coord[4][countInt]); // constructor is pitch, yaw, roll

		actor->SetActorLocation(loc);
		actor->SetActorRotation(rot);
	}
}


// Called every frame
void UPlaneInit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (play) {
		counter += 1;
		if (counter % updateNum == 0) {
			updatePlanePositions();
		}
	}

	if (counter >= 520) { // loop
		counter = 1;
	}
}

