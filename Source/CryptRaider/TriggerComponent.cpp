#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}
// Called when the game starts
void UTriggerComponent::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("Trigger alive!"));
}

void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    AActor* unlockingActor = GetAcceptableActor();
    if(unlockingActor != nullptr)
    {
        UE_LOG(LogTemp, Display, TEXT("Unlocking!"));
        if(mover!=nullptr)
        {
            UPrimitiveComponent* component = Cast<UPrimitiveComponent>(unlockingActor->GetRootComponent());
            if(component != nullptr)
            {
                component->SetSimulatePhysics(false);
            }
            unlockingActor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
            mover->SetShouldMove(true);
        }
        
    }
    else
    {
        UE_LOG(LogTemp, Display, TEXT("Locking!"));
        if(mover!=nullptr)
        {
            mover->SetShouldMove(false);
        }
    }
}

AActor* UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor*> actors;
    GetOverlappingActors(actors);
    for(AActor* actor : actors)
    {
        FString aName = actor->GetActorNameOrLabel();
        UE_LOG(LogTemp, Display, TEXT("%s"),*aName);
        if(actor->ActorHasTag(acceptableTag) && !(actor->ActorHasTag("Grabbed")))
        {
            return actor;
        }
    }
    return nullptr;
}
void UTriggerComponent::SetMover(UMover* newMover)
{
    mover = newMover;
}