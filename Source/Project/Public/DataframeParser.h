#pragma once

//#include "Project.Build.cs"
#include "CoreMinimal.h"

/**
 * To read in csv files
 * Must have columns titled with simt, type, id, lat, lon, alt, active
 * If active is 0, it is not included
 * Need to figure out how to deal with rotation
 */
class PROJECT_API DataframeParser
{
public:
	DataframeParser();
	~DataframeParser();

	// Parses data from trajectory text files to generate maps above
	void ParseData(FString&, bool cartesian);

	// For debugging
	void PrintData();

	// Returns map of coordinates
	TMap<std::int32_t, TArray<TArray<float>>> getCoordinates();


private:
	// Map of plane id to plane number
	TMap< FString, std::int32_t> planes;

	// Keep track of number of planes/plane id
	int numPlanes = 0;

	// Map of plane number to trajectory (vector of vectors containing time, x, y, z)
	TMap<std::int32_t, TArray<TArray<float>>> coordinates;

	// Adds coordinates at start and end of each plane's trajectory vector to align timing
	void FillData(float stepSize);

	// Convert from lat/lon/alt to cartesian
	TArray<float> toCartesian(float lat, float lon, float alt);

	// For fill data function
	int minSeconds = 1000000;
	int maxSeconds = -1;
};
