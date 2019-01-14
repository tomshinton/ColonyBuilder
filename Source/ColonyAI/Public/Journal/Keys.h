// ColonyBuilder Project, personal project by Tom Shinton

#pragma once

#include "CoreMinimal.h"

//////////////////////////////////////////////////////////////////////////
// All key types - allows for custom types to be stored, that would otherwise be lost in a blackboard
//////////////////////////////////////////////////////////////////////////

UENUM()
enum class EKeyType : uint8
{
	Vector,
	Guid,
	Float
};

struct FJournalKey 
{
	FJournalKey() {};

	FJournalKey(const FVector& InVector, const FName InKeyName)
		: Vector(InVector)
		, KeyName(InKeyName)
	{
		KeyType = EKeyType::Vector;
	};

	FJournalKey(const FGuid& InGuid, const FName InKeyName)
		: Guid(InGuid)
		, KeyName(InKeyName)
	{
		KeyType = EKeyType::Guid;
	}

	FJournalKey(const float& InFloat, const FName InKeyName)
		: Float(InFloat)
		, KeyName(InKeyName)
	{
		KeyType = EKeyType::Float;
	}

	bool operator==(const FJournalKey& InKey) const 
	{
		return (KeyType == InKey.KeyType) && (KeyName == InKey.KeyName);
	}

public:

	FName KeyName;
	EKeyType KeyType;

private:
	
	TOptional<FVector> Vector;
	TOptional<FGuid> Guid;
	TOptional<float> Float;

public:
	FVector GetVector() const { return Vector.GetValue(); };
	FGuid GetGuid() const { return Guid.GetValue(); };
	float GetFloat() const { return Float.GetValue(); };
};


