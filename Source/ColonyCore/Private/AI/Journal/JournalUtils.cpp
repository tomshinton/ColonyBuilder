#include "JournalUtils.h"
#include "AI/Pawns/BaseVillager.h"

DEFINE_LOG_CATEGORY_STATIC(JournalUtilsLog, Log, All);

void UJournalUtils::PushKey(FJournal& InJournal, const FJournalKey InKey)
{
	const EKeyType InKeyType = InKey.KeyType;

	const int32 IndexOfKey = InJournal.Keys.IndexOfByPredicate([&](const FJournalKey& Key)
	{
		return (InKey.KeyName == Key.KeyName) && (InKey.KeyType == Key.KeyType);
	});

	if (IndexOfKey != INDEX_NONE)
	{
		InJournal.Keys[IndexOfKey] = InKey;
	}
	else
	{
		InJournal.Keys.Add(InKey);
	}
}

FJournalKey UJournalUtils::GetKey(FJournal& InJournal, const FName InName, const EKeyType InKeyType)
{
	check(InName != "");

	const int32 KeyNum = InJournal.Keys.Num();
	if (KeyNum)
	{
		const FJournalKey* FoundKey = InJournal.Keys.FindByPredicate([&](const FJournalKey& Key)
		{ 
			return Key.KeyName == InName && Key.KeyType == InKeyType;
		});

		if (FoundKey)
		{
			return *FoundKey;
		}
	}

	return FJournalKey();
}

bool UJournalUtils::KeyExists(FJournal& InJournal, const FName InName, const EKeyType InKeyType)
{
	if (InName == "")
	{
		return false;
	}

	const int32 KeyNum = InJournal.Keys.Num();
	if (KeyNum)
	{
		const FJournalKey* FoundKey = InJournal.Keys.FindByPredicate([&](const FJournalKey& Key)
		{
			return Key.KeyName == InName && Key.KeyType == InKeyType;
		});

		if (FoundKey)
		{
			return true;
		}
	}

	return false;
}

FJournal& UJournalUtils::GetVillagerJournal(ABaseVillager* InVillager)
{
	check(InVillager);
	return InVillager->GetJournal();
}