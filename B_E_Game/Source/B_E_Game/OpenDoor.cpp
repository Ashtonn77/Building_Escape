// Copyright Ashton Naidoo 2020


#include "OpenDoor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();	
	Owner = GetOwner();

	if(!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s is missing Pressure Plate"), *GetOwner()->GetName());
	}
}

void UOpenDoor::OpenDoor()
{

	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor(){

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger volume
	//If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > 30.0f) // TODO make into parameter
	{
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//check if its time to close door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float TotalMass = 0.0f;
	
	//find all overlapping actors
	TArray<AActor*> OverlappingActors;

	// fix return value if pressure plate == nullptr
	if (!PressurePlate) { return TotalMass;  }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//iterate over them adding their masses
	for (auto *Actor : OverlappingActors)
	{
		TotalMass += (Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass());
		UE_LOG(LogTemp, Warning, TEXT("%s is on pressure plate. Total mass"), *Actor->GetName());
	}
		
	return TotalMass;
}