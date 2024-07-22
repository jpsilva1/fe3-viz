
#pragma once

#include "PlaneActor.h"
#include "TextParser.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PlaneInit.generated.h"


UCLASS()
class PROJECT_API APlaneInit : public AActor
{
	GENERATED_BODY()

private:
	// Map of plane number to plane id
	TMap<std::int32_t, FString> planes;

	// Map of plane number to trajectory (vector of vectors containing time, x, y, z)
	TMap<std::int32_t, TArray<TArray<float>>> coordinates;

	// Initializes planes from maps
	void InitPlaneActors();

	// Save planeActors in here once they are spawned
	// Can use this to update trajectories
	// Map of plane number to point of plane actor 
	TMap<std::int32_t, APlaneActor*> planeActors;

	// Method called in TickComponent to update position of planeActors
	void updatePlanePositions();

public:	
	// Sets default values for this component's properties
	APlaneInit();

	// To initialize dropdown to choose plane
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numPlanes;

	// Keep track of index of which coordinates to use
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float counter = 1;

	// To initialize slider
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int maxCount;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float getCounter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void setCounter(float num);

	// Planes move when true
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool play = false;

	// To be controlled by blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void setPlay(bool cond);

	// Change which mesh based on blueprint
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void changeMesh(const FString& input);

	// To get pointer to plane based on plane number
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	APlaneActor* getPlane(int index);


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
		
};
