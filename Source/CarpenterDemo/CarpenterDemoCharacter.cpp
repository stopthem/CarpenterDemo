// Copyright Epic Games, Inc. All Rights Reserved.

#include "CarpenterDemoCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Interfaces/InteractableInterface.h"
#include "Item/Item.h"

//////////////////////////////////////////////////////////////////////////
// ACarpenterDemoCharacter
ACarpenterDemoCharacter::ACarpenterDemoCharacter()
{
	// Create the scene component that item pickup will attach.
	ItemPickupAttachSceneComponent = CreateDefaultSubobject<USceneComponent>("ItemAttachSceneComponent");

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)
}

void ACarpenterDemoCharacter::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = GetPlayerController();
	PrevRotationalInputScales = {PlayerController->InputRollScale, PlayerController->InputPitchScale, PlayerController->InputYawScale};
}

//////////////////////////////////////////////////////////////////////////
// Input
void ACarpenterDemoCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACarpenterDemoCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACarpenterDemoCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ACarpenterDemoCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ACarpenterDemoCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACarpenterDemoCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACarpenterDemoCharacter::TouchStopped);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ACarpenterDemoCharacter::OnResetVR);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ACarpenterDemoCharacter::CheckInteract);
}

void ACarpenterDemoCharacter::CheckInteract()
{
	if (!CurrentInteractableActor)
	{
		return;
	}

	IInteractableInterface::Execute_OnInteract(CurrentInteractableActor, this);
}

void ACarpenterDemoCharacter::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (OtherActor->Implements<UInteractableInterface>())
	{
		CurrentInteractableActor = OtherActor;
	}
}

void ACarpenterDemoCharacter::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (OtherActor == CurrentInteractableActor)
	{
		IInteractableInterface::Execute_OnInteractEnd(CurrentInteractableActor, this);

		CurrentInteractableActor = nullptr;
	}
}

AItem* ACarpenterDemoCharacter::DeliverItem()
{
	if (!CarriedItem)
	{
		return nullptr;
	}

	AItem* Item = CarriedItem;
	CarriedItem = nullptr;
	return Item;
}

void ACarpenterDemoCharacter::PickupItem(AItem* Item)
{
	Item->AttachToComponent(ItemPickupAttachSceneComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	CarriedItem = Item;
}

void ACarpenterDemoCharacter::HandleRotationalInput(bool bEnable)
{
	APlayerController* PlayerController = GetPlayerController();

	PlayerController->InputRollScale = bEnable ? PrevRotationalInputScales.X : 0;
	PlayerController->InputPitchScale = bEnable ? PrevRotationalInputScales.Y : 0;
	PlayerController->InputYawScale = bEnable ? PrevRotationalInputScales.Z : 0;
}

void ACarpenterDemoCharacter::OnResetVR()
{
	// If CarpenterDemo is added to a project via 'Add Feature' in the Unreal Editor the dependency on HeadMountedDisplay in CarpenterDemo.Build.cs is not automatically propagated
	// and a linker error will result.
	// You will need to either:
	//		Add "HeadMountedDisplay" to [YourProject].Build.cs PublicDependencyModuleNames in order to build successfully (appropriate if supporting VR).
	// or:
	//		Comment or delete the call to ResetOrientationAndPosition below (appropriate if not supporting VR)
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ACarpenterDemoCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACarpenterDemoCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACarpenterDemoCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ACarpenterDemoCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ACarpenterDemoCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ACarpenterDemoCharacter::MoveRight(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
