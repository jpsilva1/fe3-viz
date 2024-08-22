#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "PythonWrapper.generated.h"

/**
 * To hopefully run the .py script to stream in Kafka data
 */
UCLASS(Blueprintable)
class PROJECT_API UPythonWrapper : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Python)
	static UPythonWrapper* Get();

	UFUNCTION(BlueprintImplementableEvent, Category = Python)
	void FunctionImplementedInPython() const;
	
	UFUNCTION(BlueprintImplementableEvent, Category = Python)
	void GetKafkaData() const;
};
