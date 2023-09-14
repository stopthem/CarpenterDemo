// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "CarpenterDemo/CarpenterDemoCharacter.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AItem::AItem()
{
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(SceneComponent);

	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionSphereComponent");
	SphereComponent->InitSphereRadius(100.0f);
	SphereComponent->SetupAttachment(SceneComponent);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	// Create dynamic material instance and set it to our mesh
	MaterialInstanceDynamic = StaticMeshComponent->CreateDynamicMaterialInstance(0);
	StaticMeshComponent->SetMaterial(0, MaterialInstanceDynamic);
}

void AItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AItem, ItemInfo);
}

void AItem::OnRep_ItemInfo()
{
	if (!MaterialInstanceDynamic)
	{
		return;
	}

	NetMC_UpdateColor();
}

void AItem::SetItemInfo(const FItemInfo& NewItemInfo)
{
	ItemInfo = NewItemInfo;
	
	// C++ OnRep only triggers clients but we are Listen as Server so we have to call manually
	OnRep_ItemInfo();
}

void AItem::SetColor(const FColor& NewColor)
{
	ItemInfo.ItemColor = NewColor;

	// C++ OnRep only triggers clients but we are Listen as Server so we have to call manually
	OnRep_ItemInfo();
}

void AItem::NetMC_UpdateColor_Implementation()
{
	MaterialInstanceDynamic->SetVectorParameterValue("Color", ItemInfo.ItemColor);
}

void AItem::OnInteract_Implementation(AActor* Interactor)
{
	IInteractableInterface::OnInteract_Implementation(Interactor);

	// Can interactor pick us up ?
	if (!Cast<ACarpenterDemoCharacter>(Interactor)->TryPickupItem(this))
	{
		return;
	}

	OnPickedUp.Broadcast();

	bPickedUp = true;
}
