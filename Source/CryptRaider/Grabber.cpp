#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

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
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UPhysicsHandleComponent *physicsHandle = GetPhysicsHandle();
	if(physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		FVector targetLocation = GetComponentLocation() + GetForwardVector()*holdDistance;
		physicsHandle->SetTargetLocationAndRotation(targetLocation,GetComponentRotation());
	}
	
	
}

void UGrabber::Grab()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if(physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("physicsHandle is nullptr!!!"));
		return;
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("got physicsHandle"));
	}
	FHitResult hitResult;
	bool hasHit = GetGrabbableInReach(hitResult);

	if(hasHit)
	{
		UPrimitiveComponent* hitComponent = hitResult.GetComponent();
		hitComponent->SetSimulatePhysics(true);
		hitComponent->WakeAllRigidBodies();
		AActor* hitActor = hitResult.GetActor();
		hitActor->Tags.Add("Grabbed");
		hitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		DrawDebugSphere(GetWorld(), hitResult.ImpactPoint, 10,10, FColor::Red, false, 5);
		physicsHandle->GrabComponentAtLocationWithRotation(
			hitComponent,
			NAME_None,
			hitResult.ImpactPoint,
			GetComponentRotation());
	}
}

void UGrabber::Release()
{
	UPhysicsHandleComponent* physicsHandle = GetPhysicsHandle();
	if(physicsHandle && physicsHandle->GetGrabbedComponent())
	{
		physicsHandle->GetGrabbedComponent()->WakeAllRigidBodies();
		AActor* grabbedActor = physicsHandle->GetGrabbedComponent()->GetOwner();
		grabbedActor->Tags.Remove("Grabbed");
		physicsHandle->ReleaseComponent();
	}
}

UPhysicsHandleComponent* UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent* physicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if(physicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("physicsHandle is nullptr!!!"));
	}
	return physicsHandle;
}

bool UGrabber::GetGrabbableInReach(FHitResult& outHitResult) const
{
	FVector start = GetComponentLocation();
	FVector end = start + GetForwardVector()*maxGrabDistance;

	DrawDebugLine(GetWorld(),start,end,FColor::Red);
	DrawDebugSphere(GetWorld(), end, 10,10, FColor::Blue, false, 5);

	FCollisionShape sphere = FCollisionShape::MakeSphere(grabRadius);

	return GetWorld()->SweepSingleByChannel(
		outHitResult,
		start,
		end,
		FQuat::Identity, 
		ECC_GameTraceChannel2,
		sphere
		);
}