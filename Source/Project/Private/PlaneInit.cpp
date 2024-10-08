#include "PlaneInit.h"
#include <UObject/UnrealTypePrivate.h>

// Sets default values for this component's properties
APlaneInit::APlaneInit() {
	bAsyncPhysicsTickEnabled = true;
}

// Initialize plane actors based on first coordinates and attitude
// Add to mapping of plane numbers to plane actors
void APlaneInit::InitPlaneActors() {
	for (auto& currPlane : coordinates) {
		int32_t planeNum = currPlane.Key;
		TArray<TArray<float>> coord = currPlane.Value;
		FVector loc = FVector(coord[1][0], coord[2][0], coord[3][0]);
		FRotator rot;
		if (rotationGiven) rot = FRotator(coord[5][0], coord[6][0], coord[4][0]); // constructor is pitch, yaw, roll
		else {
			int size = coord[0].Num() - 1;
			FVector end = FVector(coord[1][size], coord[2][size], coord[3][size]);
			rot = createRotation(loc, end);
		}
		if (!cartesian) loc = georef->TransformLongitudeLatitudeHeightPositionToUnreal(FVector(loc.Y, loc.X, loc.Z * 10));
		FActorSpawnParameters spawnInfo;
		APlaneActor* spawnedPlane = (APlaneActor*) GetWorld()->SpawnActor<APlaneActor>(APlaneActor::StaticClass(), loc, rot, spawnInfo);
		planeActors.Add(planeNum, spawnedPlane);
	}
}

// For parsing trajectory data files
// Only parses .txt files from Fe3 web portal or .csv files in specific format
void APlaneInit::inputFile_Implementation() {
	if (!active) {
		IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
		if (DesktopPlatform)
		{
			TArray<FString> OutFilenames;
			bool bOpened = DesktopPlatform->OpenFileDialog
			(	nullptr,
				TEXT("Choose File"),
				FPaths::ProjectContentDir(),
				TEXT(""),
				TEXT(".txt or .csv files"),
				EFileDialogFlags::None,
				OutFilenames );

			if (bOpened && OutFilenames.Num() > 0)
			{
				FString SelectedPath = OutFilenames[0];
				FString FileContent;

				if (FFileHelper::LoadFileToString(FileContent, *SelectedPath))
				{
					if (SelectedPath.Contains(".txt")) {
						TextParser parser;
						parser.ParseData(FileContent);
						coordinates = parser.getCoordinates();
						rotationGiven = false;
						cartesian = true;
					}
					else if (SelectedPath.Contains(".csv")) {
						DataframeParser parser;
						parser.ParseData(FileContent, false);
						coordinates = parser.getCoordinates();
						rotationGiven = false;
						cartesian = false;
					}

					InitPlaneActors();
					numPlanes = planeActors.Num();
					maxCount = coordinates[0][0].Num() - 1;
					active = true;
				}
				else {
					UE_LOG(LogTemp, Warning, TEXT("Failed to load text"));
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Failed to load file"));
			}
		}
	}
}

// Parses flight_id_mapping_list.txt from Fe3 web portal
void APlaneInit::inputVehicleFile_Implementation() {
	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
	if (DesktopPlatform)
	{
		TArray<FString> OutFilenames;
		bool bOpened = DesktopPlatform->OpenFileDialog
		(	nullptr,
			TEXT("Choose File"),
			FPaths::ProjectContentDir(),
			TEXT(""),
			TEXT(".txt files"),
			EFileDialogFlags::None,
			OutFilenames );

		if (bOpened && OutFilenames.Num() > 0)
		{
			FString SelectedPath = OutFilenames[0];
			FString FileContent;

			if (FFileHelper::LoadFileToString(FileContent, *SelectedPath))
			{
				TArray<FString> lines;
				FileContent.ParseIntoArray(lines, TEXT("\n"), false);

				for (std::int32_t i = 1; i < lines.Num(); i++) {
					TArray<FString> words;
					lines[i].ParseIntoArray(words, TEXT("\t"), false);

					if (words.Num() > 2 && planeActors.Contains(FCString::Atoi(*words[0]))) {
						FString vehicleType = "";
						if (words[2].Contains("Kinetic QuadE")) {
							vehicleType = "Quadcopter";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						} 
						else if (words[2].Contains("QuadEvtol")) {
							vehicleType = "Quadcopter";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("QuadRotorBasic")) {
							vehicleType = "Quadcopter";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("Kinetic Global Hawk")) {
							vehicleType = "Global Hawk";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("1_5_scale_decathlon")) {
							vehicleType = "3DR Aero M";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("OctoCopter_Reflection")) {
							vehicleType = "Octocopter";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("Balloon")) {
							vehicleType = "Balloon";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
						else if (words[2].Contains("MannedAircraft")) {
							vehicleType = "Bell 206";
							planeActors[FCString::Atoi(*words[0])]->setMesh(vehicleType);
						}
					}
				}
			}
			else {
				UE_LOG(LogTemp, Warning, TEXT("Failed to load text"));
			}
		}
		else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to load file"));
		}
	}
}

//
void APlaneInit::LiveData_Implementation() {
	wrapper = UPythonWrapper::Get();
	live = true;
}

void APlaneInit::updateLiveData(float DeltaTime) {
	// Run function in python, then retrieve data from csv
	wrapper->GetKafkaData();
	FString filePath = FPaths::ProjectContentDir();
	filePath.Append("Python/out.csv");
	FString FileContent;
	FFileHelper::LoadFileToString(FileContent, *filePath);
	
	// Loop through each entry and populate mapping of acId to plane actor 
	// If plane actor exists, update its position
	// If not, create a new one
	// If there is no updated entry, remove the actor from the mapping and from the scene
	const TCHAR* Terminators[] = { L"\r", L"\n" };
	const TCHAR* CSVDelimeters[] = { TEXT(",") };

	TArray<FString> CSVLines;
	FileContent.ParseIntoArray(CSVLines, Terminators, 2);

	TArray<FString> floatArr;

	for (int i = 1; i < CSVLines.Num(); i++) { // Skips first line of headers
		floatArr.Empty();
		CSVLines[i].ParseIntoArray(floatArr, CSVDelimeters, 1);
		FString acId = floatArr[3];
		float x = FCString::Atof(*floatArr[4]);
		float y = FCString::Atof(*floatArr[5]);
		float z = FCString::Atof(*floatArr[6]);
		FVector loc = FVector(x, y, z);
		loc = georef->TransformLongitudeLatitudeHeightPositionToUnreal(FVector(loc.Y, loc.X, loc.Z * 10));

		if (acId != "" && acId != "UNKN") { // remove planes without an ID since we will be using acID to keep track of them
			if (livePlaneActors.Contains(acId)) {
				APlaneActor* actor = livePlaneActors[acId];
				actor->SetActorLocation(UKismetMathLibrary::VInterpTo(actor->GetActorLocation(), loc, DeltaTime * 0.01f, 0.005f));
			}
			else {
				FRotator rot = FRotator(0.0f, 0.0f, 0.0f);
				FActorSpawnParameters spawnInfo;
				APlaneActor* spawnedPlane = (APlaneActor*)GetWorld()->SpawnActor<APlaneActor>(APlaneActor::StaticClass(), loc, rot, spawnInfo);
				livePlaneActors.Add(acId, spawnedPlane);
			}
		}
		
	}



}

// Called when the game starts
void APlaneInit::BeginPlay()
{
	Super::BeginPlay();

	
}

// Change plane positions and rotations based on counter which is updated in AsyncPhysicsTickActor
void APlaneInit::updatePlanePositions(float DeltaTime) {
	for (auto& curr : planeActors) {
		std::int32_t planeNum = curr.Key;
		APlaneActor* actor = curr.Value;
		TArray<TArray<float>>& coord = coordinates[planeNum];
		int32_t countInt = counter;
		FVector loc = FVector(coord[1][countInt], coord[2][countInt], coord[3][countInt]);

		if (cartesian) actor->SetActorLocation(loc);
		else {
			loc = georef->TransformLongitudeLatitudeHeightPositionToUnreal(FVector(loc.Y, loc.X, loc.Z * 10)); 
			actor->SetActorLocation(UKismetMathLibrary::VInterpTo(actor->GetActorLocation(), loc, DeltaTime * 0.01f, 0.005f)); // Uses interpolation because lat/lon points more spread out than cartesian coordinates
		}
	}
}

// Finds rotation between first and last point in plane's trajectory
FRotator APlaneInit::createRotation(FVector start, FVector end) {
	if (!cartesian) {
		start = georef->TransformLongitudeLatitudeHeightPositionToUnreal(FVector(start.Y, start.X, start.Z * 10));
		end = georef->TransformLongitudeLatitudeHeightPositionToUnreal(FVector(end.Y, end.X, end.Z * 10));
	} 
	return UKismetMathLibrary::MakeRotFromXZ(start - end, FVector::UpVector);
}

// Basically like FixedUpdate, moves all the planes based on time
void APlaneInit::AsyncPhysicsTickActor(float DeltaTime, float SimTime) {
	if (active && play && cartesian) {
		counter += currStatus;
		if (counter >= maxCount) counter = 1; // Loop back to starting coordinate
		updatePlanePositions(SimTime);
	}
	else if (active && play && !cartesian) {
		if (latLonCounter % 100 == 0) { // Updates non-cartesian coordinates slower since they are farther apart
			counter += currStatus;
			if (counter >= maxCount) counter = 1;
			updatePlanePositions(SimTime);
		}
		else latLonCounter++;
	}
	else if (live) {
		updateLiveData(SimTime);
	}
}

float APlaneInit::getCounter_Implementation() {
	return counter;
}

void APlaneInit::setCounter_Implementation(const float& num) {
	counter = num;
}

void APlaneInit::setPlay_Implementation(bool cond) {
	play = cond;
}

// Sets mesh of every plane in scene
void APlaneInit::changeMesh_Implementation(const FString& input) {
	for (auto& curr : planeActors) {
		APlaneActor* actor = curr.Value;
		actor->setMesh(input);
	}
}

APlaneActor* APlaneInit::getPlane_Implementation(int index) {
	return planeActors[index];
}

bool APlaneInit::getPathChanged_Implementation() {
	return pathChanged;
}

void APlaneInit::setPathChanged_Implementation(bool input) {
	pathChanged = input;
}