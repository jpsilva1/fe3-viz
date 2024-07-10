// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlaneInit.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_API UPlaneInit : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlaneInit();

	// Map of plane number to plane id
	TMap<std::int32_t, FString> planes;

	// Map of plane number to trajectory (vector of vectors containing time, lat, lon, alt)
	TMap<std::int32_t, TArray<TArray<float>>> coordinates;

	// Parses data from trajectory text files to generate maps above
	void ParseData(FString&);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
		
};
