#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "PlaneActor.generated.h"


UCLASS()
class PROJECT_API APlaneActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APlaneActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void setMesh(FString input);

	UPROPERTY()
	USceneComponent* Root;
	
	UPROPERTY()
	UStaticMesh* PlaneMesh;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

};
