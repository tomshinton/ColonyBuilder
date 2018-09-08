#pragma once
#include <NameTypes.h>
#include <ObjectMacros.h>
#include <BehaviorTree/BlackboardComponent.h>
#include <BehaviorTree/BlackboardData.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Bool.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Class.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Float.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Int.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Name.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Rotator.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_String.h>
#include <BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>

#include "PackedBlackboard.generated.h"

// PackedBlackboard data type, used to remove abstraction in regards to uint8 arrays for Blackboard data types
//
// Supported types:
// Bool
// UObject Subclasses
// floats
// int32
// FName
// FRotator
// FString
// FVector

#pragma region Keys
USTRUCT()
struct FPackedKey
{
	GENERATED_BODY()

	FPackedKey()
		: KeyName()
	{}

	FPackedKey(const FName InName)
		: KeyName(InName)
	{}
	UPROPERTY()
	FName KeyName;

	bool operator==(const FBlackboardEntry& BlackboardKey) const
	{
		return BlackboardKey.EntryName == KeyName;
	}

	bool operator==(const FPackedKey& OtherPackedKey) const
	{
		return OtherPackedKey.KeyName == KeyName;
	}
};
USTRUCT()
struct FPackedBool : public FPackedKey
{
	GENERATED_BODY()

	FPackedBool()
		: FPackedKey()
		, Value(false)
	{}

	FPackedBool(const FName InName, const bool InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	bool Value;
};
USTRUCT()
struct FPackedClass : public FPackedKey
{
	GENERATED_BODY()

	FPackedClass()
		: FPackedKey()
		, Value()
	{}

	FPackedClass(const FName InName, const TSubclassOf<UObject> InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	TSubclassOf<UObject> Value;
};
USTRUCT()
struct FPackedFloat : public FPackedKey
{
	GENERATED_BODY()

	FPackedFloat()
		: FPackedKey()
		, Value()
	{}

	FPackedFloat(const FName InName, const float InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	float Value;
};
USTRUCT()
struct FPackedInt : public FPackedKey
{
	GENERATED_BODY()

	FPackedInt()
		: FPackedKey()
		, Value()
	{}

	FPackedInt(const FName InName, const int32 InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	int32 Value;
};
USTRUCT()
struct FPackedName : public FPackedKey
{
	GENERATED_BODY()

	FPackedName()
		: FPackedKey()
		, Value()
	{}

	FPackedName(const FName InName, const FName InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	FName Value;
};
USTRUCT()
struct FPackedRotator : public FPackedKey
{
	GENERATED_BODY()

	FPackedRotator()
		: FPackedKey()
		, Value()
	{}

	FPackedRotator(const FName InName, const FRotator InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	FRotator Value;
};
USTRUCT()
struct FPackedString : public FPackedKey
{
	GENERATED_BODY()

	FPackedString()
		: FPackedKey()
		, Value()
	{}

	FPackedString(const FName InName, const FString InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	FString Value;
};
USTRUCT()
struct FPackedVector : public FPackedKey
{
	GENERATED_BODY()

	FPackedVector()
		: FPackedKey()
		, Value()
	{}

	FPackedVector(const FName InName, const FVector InValue)
		: FPackedKey(InName)
		, Value(InValue)
	{}
	UPROPERTY()
	FVector Value;
};
#pragma endregion

USTRUCT()
struct COLONYAI_API FPackedBlackboard
{
	GENERATED_BODY()

		bool GetBoolByName(const FName InName)
	{
		for (FPackedBool& PackedBool : PackedBools)
		{
			if (PackedBool.KeyName == InName)
			{
				return PackedBool.Value;
			}
		}

		return false;
	}

	TSubclassOf<UObject> GetClassByName(const FName InName)
	{
		for (FPackedClass& PackedClass : PackedClasses)
		{
			if (PackedClass.KeyName == InName)
			{
				return PackedClass.Value;
			}
		}

		return nullptr;
	}

	float GetFloatByName(const FName InName)
	{
		for (FPackedFloat& PackedFloat : PackedFloats)
		{
			if (PackedFloat.KeyName == InName)
			{
				return PackedFloat.Value;
			}
		}

		return 0.f;
	}

	int32 GetIntByName(const FName InName)
	{
		for (FPackedInt& PackedInt : PackedInts)
		{
			if (PackedInt.KeyName == InName)
			{
				return PackedInt.Value;
			}
		}

		return 0;
	}

	FName GetNameByName(const FName InName)
	{
		for (FPackedName& PackedName : PackedNames)
		{
			if (PackedName.KeyName == InName)
			{
				return PackedName.Value;
			}
		}

		return TEXT("");
	}

	FRotator GetRotatorByName(const FName InName)
	{
		for (FPackedRotator& PackedRotator : PackedRotators)
		{
			if (PackedRotator.KeyName == InName)
			{
				return PackedRotator.Value;
			}
		}

		return FRotator::ZeroRotator;
	}

	FString GetStringByName(const FName InName)
	{
		for (FPackedString& PackedString : PackedStrings)
		{
			if (PackedString.KeyName == InName)
			{
				return PackedString.Value;
			}
		}

		return TEXT("");
	}

	FVector GetVectorByName(const FName InName)
	{
		for (FPackedVector& PackedVector : PackedVectors)
		{
			if (PackedVector.KeyName == InName)
			{
				return PackedVector.Value;
			}
		}

		return FVector::ZeroVector;
	}

	void Pack(UBlackboardComponent* SourceBlackboardComponent)
	{
		if (!SourceBlackboardComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("No source Blackboard Component supplied to PackedBlackboard::Pack() - aborting"));
			return;
		}

		PackedBools.Reset();
		PackedClasses.Reset();
		PackedFloats.Reset();
		PackedInts.Reset();
		PackedNames.Reset();
		PackedRotators.Reset();
		PackedStrings.Reset();
		PackedVectors.Reset();

		TArray<FBlackboardEntry> FoundKeys;

		FoundKeys = SourceBlackboardComponent->GetBlackboardAsset()->Keys;

		if (FoundKeys.Num() > 0)
		{
			for (FBlackboardEntry& Key : FoundKeys)
			{
				UBlackboardKeyType* CurrentType = Key.KeyType;

				if (Cast<UBlackboardKeyType_Bool>(CurrentType))
				{
					PackedBools.Add(FPackedBool(Key.EntryName, SourceBlackboardComponent->GetValueAsBool(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Class>(CurrentType))
				{
					PackedClasses.Add(FPackedClass(Key.EntryName, SourceBlackboardComponent->GetValueAsClass(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Float>(CurrentType))
				{
					PackedFloats.Add(FPackedFloat(Key.EntryName, SourceBlackboardComponent->GetValueAsFloat(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Int>(CurrentType))
				{
					PackedInts.Add(FPackedInt(Key.EntryName, SourceBlackboardComponent->GetValueAsInt(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Name>(CurrentType))
				{
					PackedNames.Add(FPackedName(Key.EntryName, SourceBlackboardComponent->GetValueAsName(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Rotator>(CurrentType))
				{
					PackedRotators.Add(FPackedRotator(Key.EntryName, SourceBlackboardComponent->GetValueAsRotator(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_String>(CurrentType))
				{
					PackedStrings.Add(FPackedString(Key.EntryName, SourceBlackboardComponent->GetValueAsString(Key.EntryName)));
					continue;
				}
				if (Cast<UBlackboardKeyType_Vector>(CurrentType))
				{
					PackedVectors.Add(FPackedVector(Key.EntryName, SourceBlackboardComponent->GetValueAsVector(Key.EntryName)));
					continue;
				}

				UE_LOG(LogTemp, Error, TEXT("Unsupported Blackboard Type detected in PackedBlackboard"));
			}
		}
	}

	void Unpack(UBlackboardComponent* OutBlackboardComponent)
	{
		if (!OutBlackboardComponent)
		{
			UE_LOG(LogTemp, Warning, TEXT("No source Blackboard Component supplied to PackedBlackboard::Unpack() - aborting"));
			return;
		}

		for (FBlackboardEntry& Key : OutBlackboardComponent->GetBlackboardAsset()->Keys)
		{
			UBlackboardKeyType* CurrentType = Key.KeyType;

			if (Cast<UBlackboardKeyType_Bool>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsBool(Key.EntryName, GetBoolByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Class>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsClass(Key.EntryName, GetClassByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Float>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsFloat(Key.EntryName, GetFloatByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Int>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsInt(Key.EntryName, GetIntByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Name>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsName(Key.EntryName, GetNameByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Rotator>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsRotator(Key.EntryName, GetRotatorByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_String>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsString(Key.EntryName, GetStringByName(Key.EntryName));
				continue;
			}
			if (Cast<UBlackboardKeyType_Vector>(CurrentType))
			{
				OutBlackboardComponent->SetValueAsVector(Key.EntryName, GetVectorByName(Key.EntryName));
				continue;
			}
		}
	}


	UPROPERTY(SaveGame)
		TArray<FPackedBool> PackedBools;
	UPROPERTY(SaveGame)
		TArray<FPackedClass> PackedClasses;
	UPROPERTY(SaveGame)
		TArray<FPackedFloat> PackedFloats;
	UPROPERTY(SaveGame)
		TArray<FPackedInt> PackedInts;
	UPROPERTY(SaveGame)
		TArray<FPackedName> PackedNames;
	UPROPERTY(SaveGame)
		TArray<FPackedRotator> PackedRotators;
	UPROPERTY(SaveGame)
		TArray<FPackedString> PackedStrings;
	UPROPERTY(SaveGame)
		TArray<FPackedVector> PackedVectors;
};