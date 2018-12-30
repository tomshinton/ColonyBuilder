// ColonyBuilder Project, personal project by Tom Shinton
#include "EnvQueryTest_InvertedOverlap.h"

#include "EnvironmentQuery/Tests/EnvQueryTest_Overlap.h"
#include "UObject/Package.h"
#include "CollisionQueryParams.h"
#include "WorldCollision.h"
#include "Engine/World.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"

UEnvQueryTest_InvertedOverlap::UEnvQueryTest_InvertedOverlap()
{
	Cost = EEnvTestCost::High;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_InvertedOverlap::RunTest(FEnvQueryInstance& QueryInstance) const
{
	FCollisionQueryParams OverlapParams(SCENE_QUERY_STAT(EnvQueryOverlap), false);
	OverlapParams.bTraceAsyncScene = true;

	for (FEnvOverlapData OverlapInfo : OverlapData)
	{
		const ECollisionChannel OverlapCollisionChannel = OverlapInfo.OverlapChannel;
		const FVector TraceExtent(OverlapInfo.ExtentX, OverlapInfo.ExtentY, OverlapInfo.ExtentZ);

		FCollisionShape CollisionShape;
		switch (OverlapInfo.OverlapShape)
		{
		case EEnvOverlapShape::Box:
			CollisionShape = FCollisionShape::MakeBox(TraceExtent);
			break;

		case EEnvOverlapShape::Sphere:
			CollisionShape = FCollisionShape::MakeSphere(TraceExtent.X);
			break;

		case EEnvOverlapShape::Capsule:
			CollisionShape = FCollisionShape::MakeCapsule(TraceExtent.X, TraceExtent.Z);
			break;

		default:
			return;
		}

		FRunOverlapSignatureInverted OverlapFunc;
		if (OverlapInfo.bOnlyBlockingHits)
		{
			OverlapFunc.BindUObject(this, &UEnvQueryTest_InvertedOverlap::RunOverlapBlocking);
		}
		else
		{
			OverlapFunc.BindUObject(this, &UEnvQueryTest_InvertedOverlap::RunOverlap);
		}

		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
			AActor* ItemActor = GetItemActor(QueryInstance, It.GetIndex());

			const bool bHit = OverlapFunc.Execute(ItemLocation + OverlapInfo.ShapeOffset, CollisionShape, ItemActor, QueryInstance.World, OverlapCollisionChannel, OverlapParams);

			float ItemScore = bHit ? 0.f : 1.f;

			It.SetScore(TestPurpose, FilterType, ItemScore, 0.f, 1.f);
		}
	}
}

FText UEnvQueryTest_InvertedOverlap::GetDescriptionTitle() const
{
	FString DescriptionTitle = "Filtering points outside of specified overlap types:";

	return FText::FromString(DescriptionTitle);
}

FText UEnvQueryTest_InvertedOverlap::GetDescriptionDetails() const
{
	FString OverlapTypes;

	if (OverlapData.Num() > 0)
	{
		for (int32 i = 0; i < OverlapData.Num(); i++)
		{
			UEnum* ChannelEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ECollisionChannel"), true);
			FString ChannelDesc = ChannelEnum->GetDisplayNameTextByValue(OverlapData[i].OverlapChannel).ToString();

			OverlapTypes += ", " + ChannelDesc;
		}
	}

	return FText::FromString(OverlapTypes);
}

bool UEnvQueryTest_InvertedOverlap::RunOverlap(const FVector& ItemPos, const FCollisionShape& CollisionShape, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params)
{
	FCollisionQueryParams OverlapParams(Params);
	OverlapParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->OverlapAnyTestByChannel(ItemPos, FQuat::Identity, Channel, CollisionShape, OverlapParams);
	return bHit;
}

bool UEnvQueryTest_InvertedOverlap::RunOverlapBlocking(const FVector& ItemPos, const FCollisionShape& CollisionShape, AActor* ItemActor, UWorld* World, enum ECollisionChannel Channel, const FCollisionQueryParams& Params)
{
	FCollisionQueryParams OverlapParams(Params);
	OverlapParams.AddIgnoredActor(ItemActor);

	const bool bHit = World->OverlapBlockingTestByChannel(ItemPos, FQuat::Identity, Channel, CollisionShape, OverlapParams);
	return bHit;
}
