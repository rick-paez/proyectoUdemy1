// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"


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

	// ...
	Owner = GetOwner();
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

void UOpenDoor::OpenDoor()
{
	//UE_LOG(LogTemp, Warning, TEXT("La rotacion de %s es %s"), *strObjectNAme, *strRotation);
	Owner->SetActorRotation(FRotator(0, OpenAngle, 0));
	//UE_LOG(LogTemp, Warning, TEXT("La rotacion de %s es %s"), *strObjectNAme, *strRotation);
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0, -90, 0));
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Polleamos el trigger volume
	// Si el actor que abre esta en el volumen entonces abrimos la puerta
	if (PressurePlate->IsOverlappingActor(ActorThatOpens)) {
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	//Checar si se cerrará la puerta

	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime >DoorCloseDelay) {
		CloseDoor();
	}
	

}

