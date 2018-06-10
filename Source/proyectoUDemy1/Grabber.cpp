// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"



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
	// ...
	FindPhysicsComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// if the physics handle is attached
		/// move the object that we're holding
	if (PhysicsHandle->GrabbedComponent) {
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
			OUT PlayerViewPointLocation,
			OUT PlayerViewPointRotation);
		LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
	

	// See what 
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
	/// LINETRACE Try and reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	///  If we hit someething then attach a physics handle
	if (ActorHit) {	
		
		PhysicsHandle->GrabComponent(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true);
		/*PhysicsHandle->GrabComponentAtLocation(ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation());*/
	}


}
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));
	PhysicsHandle->ReleaseComponent();
}

void UGrabber::FindPhysicsComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("PhysicsHandle found in %s"), *GetOwner()->GetName());
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("PhysicsHandle not found in %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	///Look for InputComponent 
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent found in %s"), *GetOwner()->GetName());
		///Bind the  pressed event
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		///Bind the released event
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent not found in %s"), *GetOwner()->GetName());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// get player Viewpoint this tick
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation);
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	//UE_LOG(LogTemp, Warning, TEXT("LineTraceEnd %s"), *LineTraceEnd.ToString());
	/*DrawDebugLine(GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.f);*/
	///Setup collision queryParameter
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	///Ray cast 
	FHitResult Hit;
	///
	if (GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	)) {
		UE_LOG(LogTemp, Warning, TEXT("Collisioned with: %s"), *Hit.GetActor()->GetName());
	}

	return Hit;
}
