#include "TextParser.h"

TextParser::TextParser()
{
}

TextParser::~TextParser()
{
}

TMap<std::int32_t, FString> TextParser::getPlanes() {
	return planes;
}

TMap<std::int32_t, TArray<TArray<float>>> TextParser::getCoordinates() {
	return coordinates;
}


void TextParser::ParseData(FString& input) {
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
			float roll = (FCString::Atof(*(attitude.Mid(0, 2))) * 2) - 90.0f;
			float pitch = (FCString::Atof(*(attitude.Mid(2, 2))) * 2) - 90.0f;
			float yaw = FCString::Atof(*(attitude.Mid(4, 2)));

			TArray<TArray<float>>& coord = coordinates[planeNum];
			coord[0].Add(seconds);
			coord[1].Add(x);
			coord[2].Add(y);
			coord[3].Add(z);
			coord[4].Add(roll);
			coord[5].Add(pitch);
			coord[6].Add(yaw);
		}

	}
	FillData();
}


// Change trajectory vectors so each one starts with 0.5 and ends with 275
void TextParser::FillData() {
	for (auto& curr : planes) {
		TArray<TArray<float>>& coord = coordinates[curr.Key];
		float startTime = coord[0][0];
		float endTime = coord[0][519];
		if (startTime > 0.5) {
			float x = coord[1][0];
			float y = coord[2][0];
			float z = coord[3][0];
			float roll = coord[4][0];
			float pitch = coord[5][0];
			float yaw = coord[6][0];
			for (float i = startTime - 0.5f; i > 0.5f; i = i - 0.5f) {
				coord[0].Insert(i, 0);
				coord[1].Insert(x, 0);
				coord[2].Insert(y, 0);
				coord[3].Insert(z, 0);
				coord[4].Insert(roll, 0);
				coord[5].Insert(pitch, 0);
				coord[6].Insert(yaw, 0);
			}
		}

		if (endTime < 275.0f) {
			float x = coord[1][519];
			float y = coord[2][519];
			float z = coord[3][519];
			float roll = coord[4][519];
			float pitch = coord[5][519];
			float yaw = coord[6][519];
			for (float i = endTime + 0.5f; i < 275.0f; i = i + 0.5f) {
				int currLen = coord[0].Num();
				coord[0].Insert(i, currLen);
				coord[1].Insert(x, currLen);
				coord[2].Insert(y, currLen);
				coord[3].Insert(z, currLen);
				coord[4].Insert(roll, currLen);
				coord[5].Insert(pitch, currLen);
				coord[6].Insert(yaw, currLen);
			}
		}
	}

}

void TextParser::PrintData() {
	// just print the first one
	TArray<TArray<float>>& coord = coordinates[0];

	for (int i = 0; i < coord[0].Num(); i++) {
		float seconds = coord[0][i];
		float x = coord[1][i];
		float y = coord[2][i];
		float z = coord[3][i];
		float roll = coord[4][i];
		float pitch = coord[5][i];
		float yaw = coord[6][i];

		UE_LOG(LogTemp, Warning, TEXT("sec: %f, x: %f, y: %f, z: %f, r: %f, p: %f, y: %f"), seconds, x, y, z, roll, pitch, yaw);
	}

}
