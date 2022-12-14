#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Mover.h"
#include "TriggerComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class CRYPTRAIDER_API UTriggerComponent : public UBoxComponent
{
	GENERATED_BODY()
public:
	UTriggerComponent();
protected:
	virtual void BeginPlay() override;
public:	
	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void SetMover(UMover* newMover);
private:
	UPROPERTY(EditAnywhere)
	FName acceptableTag;
	UMover* mover;
	AActor* GetAcceptableActor() const;
};
