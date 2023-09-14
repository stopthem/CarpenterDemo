// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "CarpenterDemo/CarpenterDemoCharacter.h"
#include "Components/SphereComponent.h"

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

void AItem::SetItemInfo(const FItemInfo& NewItemInfo)
{
	ItemInfo = NewItemInfo;

	UpdateColor();
}

void AItem::SetColor(const FColor& NewColor)
{
	ItemInfo.ItemColor = NewColor;
	UpdateColor();
}

void AItem::UpdateColor() const
{
	MaterialInstanceDynamic->SetVectorParameterValue("Color", ItemInfo.ItemColor);
}

void AItem::OnInteract_Implementation(AActor* Interactor)
{
	IInteractableInterface::OnInteract_Implementation(Interactor);

	if (bPickedUp)
	{
		return;
	}

	Cast<ACarpenterDemoCharacter>(Interactor)->PickupItem(this);

	OnPickedUp.Broadcast();

	bPickedUp = true;
}
