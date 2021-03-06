// ColonyBuilder Project, personal project by Tom Shinton

#include "ConstructionSiteComponent.h"
#include "ConstructorHelpers.h"

#include "BaseVillager.h"

UConstructionSiteComponent::UConstructionSiteComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision Component"));
	//CollisionComponent->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	CollisionComponent->SetBoxExtent(FVector(200, 200, 200), true);
	CollisionComponent->SetCollisionProfileName("OverlapAllDynamic");

	Sprite = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	//Sprite->AttachToComponent(this, FAttachmentTransformRules::KeepRelativeTransform, NAME_None);
	Sprite->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));

	ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTextureRef(TEXT("Texture2D'/Game/Resources/Sprites/SP_ConstructionSite.SP_ConstructionSite'"));
	if (SpriteTextureRef.Object)
	{
		Sprite->Sprite = SpriteTextureRef.Object;
	}
}