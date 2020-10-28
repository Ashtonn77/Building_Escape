// Copyright Ashton Naidoo 2020


#include "Grabber.h"
#include "DrawDebugHelpers.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty"));

	// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing physics handle component"), *GetOwner()->GetName());
	}


	// Look for attached input handle at runtime
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		// Bind Input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing input component"), *GetOwner()->GetName());
	}
	
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Button released"));
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Get player viewpoint this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotator;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotator
	);

	//log out viewpoint
	//UE_LOG(LogTemp, Warning, TEXT("Location: %s, Rotation: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotator.ToString());

	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotator.Vector() * Reach;
	//draw a red trace in the world to visualize
	DrawDebugLine(
	
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.0f,
		0.0f,
		10.0f
	
	);

	//setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());


	// Ray-cast/Line-trace out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(

		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters

	);

	// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You just hit the %s"), *(ActorHit->GetName()));
	}
}

