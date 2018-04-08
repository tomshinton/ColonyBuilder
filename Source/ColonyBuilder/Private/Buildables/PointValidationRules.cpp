// ColonyBuilder Project, personal project by Tom Shinton

#include "PointValidationRules.h"

#include "Ghost.h"
#include "Runtime/Landscape/Classes/Landscape.h"

UPointValidationRules::UPointValidationRules()
{
	//Add allowed overlap classes
	AllowedOverlapClasses.AddUnique(AGhost::StaticClass());
	AllowedOverlapClasses.AddUnique(ALandscape::StaticClass());
}
