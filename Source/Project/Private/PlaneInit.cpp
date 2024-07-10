// Copyright 2020-2021 CesiumGS, Inc. and Contributors


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
	// loop through the rest of the lines which are time in seconds, plane number, lat, lon, alt 

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
			TArray<float> lat;
			TArray<float> lon;
			TArray<float> alt;
			TArray<TArray<float>> coord = { seconds, lat, lon, alt };
			coordinates.Add(FCString::Atoi(*words[0]), coord);
		}
		else {
			// add to mapping of coordinates
			// line should have format time, plane number, lat, lon, alt
			float seconds = FCString::Atof(*words[0]);
			int32_t planeNum = FCString::Atoi(*words[1]);
			float lat = FCString::Atof(*words[2]);
			float lon = FCString::Atof(*words[3]);
			float alt = FCString::Atof(*words[4]);

			TArray<TArray<float>> &coord = coordinates[planeNum];
			coord[0].Add(seconds);
			coord[1].Add(lat);
			coord[2].Add(lon);
			coord[3].Add(alt);
		}

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
	
}


// Called every frame
void UPlaneInit::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

