// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "CarpenterDemo/CarpenterDemoCharacter.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"

AItem::AItem()
{
	// Create root component
	SceneComponent = CreateDefaultSubobject<USceneComponent>("SceneComponent");
	SetRootComponent(SceneComponent);

	// Create mesh and attach to root
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	StaticMeshComponent->SetupAttachment(SceneComponent);

	// Create sphere component for interaction and attach to root
	SphereComponent = CreateDefaultSubobject<USphereComponent>("InteractionSphereComponent");
	SphereComponent->InitSphereRadius(100.0f);
	SphereComponent->SetupAttachment(SceneComponent);
	
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = false;
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

void AItem::OnRep_ItemInfo() const
{
	if (!MaterialInstanceDynamic)
	{
		return;
	}

	Nmc_UpdateColor();
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

void AItem::Nmc_UpdateColor_Implementation() const
{
	MaterialInstanceDynamic->SetVectorParameterValue("Color", ItemInfo.ItemColor);
}

void AItem::PickedUp_Implementation()
{
	OnPickedUp.Broadcast();

	bPickedUp = true;
}

void AItem::OnInteract_Implementation(AActor* Interactor)
{
	IInteractableInterface::OnInteract_Implementation(Interactor);

	ACarpenterDemoCharacter* CarpenterDemoCharacter = Cast<ACarpenterDemoCharacter>(Interactor);
	
	// Can interactor pick us up ?
	if (CarpenterDemoCharacter->GetItem() || bPickedUp)
	{
		return;
	}

	CarpenterDemoCharacter->Server_PickupItem(this);
}
