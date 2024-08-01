#include "DataframeParser.h"

DataframeParser::DataframeParser()
{
}

DataframeParser::~DataframeParser()
{
}

TArray<float> DataframeParser::toCartesian(float lat, float lon, float alt) {
	float radius = 6371 + alt; // radius of earth added to elevation
	float x = radius * FMath::Cos(lat) * FMath::Cos(lon);
	float y = radius * FMath::Cos(lat) * FMath::Sin(lon);
	float z = radius * FMath::Sin(lat);
	TArray<float> result = { x, y, z };
	return result;

}

void DataframeParser::ParseData(FString& input, bool cartesian) {
	TArray<TArray<float>> parsedCSV;

	const TCHAR* Terminators[] = { L"\r", L"\n" }; 
	const TCHAR* CSVDelimeters[] = { TEXT(",") };

	TArray<FString> CSVLines;
	input.ParseIntoArray(CSVLines, Terminators, 2);

	TArray<FString> floatArr;

	//ACesiumGeoreference* georef;

	//REMEMBER TO CHANGE BACK TO CSVLines.Num()
	for (int i = 1; i < 1000; i++) { // make sure to skip first line of headers
		floatArr.Empty();
		CSVLines[i].ParseIntoArray(floatArr, CSVDelimeters, 1);

		if (floatArr[7] == "1") { // remove non-active planes
			if (!planes.Contains(floatArr[1])) {
				// init vectors into map
				TArray<float> seconds;
				TArray<float> lat;
				TArray<float> lon;
				TArray<float> alt;
				TArray<TArray<float>> coord = { seconds, lat, lon, alt };

				coordinates.Add(numPlanes, coord);
				planes.Add(floatArr[1], numPlanes);
				numPlanes++;
			}

			// Add onto current trajectory of plane
			int currPlane = planes[floatArr[1]];
			TArray<TArray<float>>& coord = coordinates[currPlane];

			float seconds = FCString::Atof(*floatArr[0]);
			if (seconds < minSeconds) minSeconds = seconds;
			if (seconds > maxSeconds) maxSeconds = seconds;
			float x = FCString::Atof(*floatArr[3]);
			float y = FCString::Atof(*floatArr[4]);
			float z = FCString::Atof(*floatArr[5]);

			if (!cartesian) {
				//TArray<float> result = toCartesian(x, y, z);
				//x = result[0];
				//y = result[1];
				//z = result[2];
			}

			coord[0].Add(seconds);
			coord[1].Add(x);
			coord[2].Add(y);
			coord[3].Add(z);
		}
		
	}
	int lastNum = coordinates.Num() - 1;
	TArray<TArray<float>> lastCoord = coordinates[lastNum];
	FillData(1.0f);
}

void DataframeParser::FillData(float stepSize) {
	for (auto& curr : coordinates) {
		TArray<TArray<float>>& coord = coordinates[curr.Key];
		float startTime = coord[0][0];
		float endTime = coord[0][coord[0].Num() - 1];
		if (startTime > minSeconds) {
			float x = coord[1][0];
			float y = coord[2][0];
			float z = coord[3][0];
			for (float i = startTime - stepSize; i >= minSeconds; i = i - stepSize) {
				coord[0].Insert(i, 0);
				coord[1].Insert(x, 0);
				coord[2].Insert(y, 0);
				coord[3].Insert(z, 0);
			}
		}

		if (endTime < maxSeconds) {
			int num = coord[0].Num() - 1;
			float x = coord[1][num];
			float y = coord[2][num];
			float z = coord[3][num];
			for (float i = endTime + stepSize; i <= maxSeconds; i = i + stepSize) {
				int currLen = coord[0].Num();
				coord[0].Insert(i, currLen);
				coord[1].Insert(x, currLen);
				coord[2].Insert(y, currLen);
				coord[3].Insert(z, currLen);
			}
		}
	}

}

void DataframeParser::PrintData() {
	TArray<TArray<float>>& coord = coordinates[0];

	for (int i = 0; i < coord[0].Num(); i++) {
		float seconds = coord[0][i];
		float x = coord[1][i];
		float y = coord[2][i];
		float z = coord[3][i];

		UE_LOG(LogTemp, Warning, TEXT("sec: %f, x: %f, y: %f, z: %f"), seconds, x, y, z);
	}

	
	TArray<TArray<float>>& coord1 = coordinates[3];

	for (int i = 0; i < coord1[0].Num(); i++) {
		float seconds = coord1[0][i];
		float x = coord1[1][i];
		float y = coord1[2][i];
		float z = coord1[3][i];

		UE_LOG(LogTemp, Warning, TEXT("sec: %f, x: %f, y: %f, z: %f"), seconds, x, y, z);
	}
}

TMap<std::int32_t, TArray<TArray<float>>> DataframeParser::getCoordinates() {
	return coordinates;
}
