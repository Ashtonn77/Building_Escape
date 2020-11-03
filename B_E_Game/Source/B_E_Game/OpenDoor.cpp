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

void UOpenDoor::CloseDoor(){

	Owner->SetActorRotation(FRotator(0.f, 0.f, 0.f));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Poll the trigger volume
	//If the ActorThatOpens is in the volume
	if (GetTotalMassOfActorsOnPlate() > TriggerMass) // TODO make into parameter
	{
		OnOpen.Broadcast();
	}
	else 
	{
		OnClose.Broadcast();
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