#pragma once

#include "PlaneActor.h"
#include "CoreMinimal.h"

/**
 * For parsing .txt files
 * must be in format from fe3.arc.nasa.gov
 */
class PROJECT_API TextParser
{
public:
	TextParser();
	~TextParser();

	// Parses data from trajectory text files to generate maps above
	void ParseData(FString&);

	// For debugging
	void PrintData();

	void printError();

	// Returns map of plane number to plane id
	TMap<std::int32_t, FString> getPlanes();

	// Returns map of coordinates
	TMap<std::int32_t, TArray<TArray<float>>> getCoordinates();


private:
	// Map of plane number to plane id
	TMap<std::int32_t, FString> planes;

	// Map of plane number to trajectory (vector of vectors containing time, x, y, z)
	TMap<std::int32_t, TArray<TArray<float>>> coordinates;

	// Adds coordinates at start and end of each plane's trajectory vector to align timing
	void FillData(float stepSize);

	// For fill data function
	int minSeconds = 1000000;
	int maxSeconds = -1;

};
