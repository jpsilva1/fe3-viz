#include "PythonWrapper.h"

UPythonWrapper* UPythonWrapper::Get() {
	TArray<UClass*> PythonWrapperClasses;
	GetDerivedClasses(UPythonWrapper::StaticClass(), PythonWrapperClasses);
	int32_t numClasses = PythonWrapperClasses.Num();
	if (numClasses > 0) {
		return Cast<UPythonWrapper>(PythonWrapperClasses[numClasses - 1]->GetDefaultObject());
	}
	return nullptr;

	// to run python functions:
	// UPythonWrapper* wrapper = UPythonWrapper::Get();
	// bridge->FunctionImplementedInPython
}