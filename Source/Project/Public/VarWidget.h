#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "VarWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT_API UVarWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool planeInMotion;

	bool getPlaneInMotion();
	
};
