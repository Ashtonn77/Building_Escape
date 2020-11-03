// Copyright Ashton Naidoo 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine\TriggerVolume.h"
#include "OpenDoor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDoorEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class B_E_GAME_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;

	void CloseDoor();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable);
	FDoorEvent OnOpen;

	UPROPERTY(BlueprintAssignable);
	FDoorEvent OnClose;

private:
	
		UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;

		UPROPERTY(EditAnywhere)
		float TriggerMass = 30.0f;

		AActor* Owner = GetOwner();

		//returns total mass in kg
		float GetTotalMassOfActorsOnPlate();
		
};
