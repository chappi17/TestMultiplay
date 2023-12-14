#include "TranspoterComponent.h"

UTranspoterComponent::UTranspoterComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	SetIsReplicatedByDefault(true);

	MoveTime = 3.0f;
	ActivatedTriggerCount = 0;

	ArePointsSet = false;
	StartPoint = FVector::Zero();
	EndPoint = FVector::Zero();
}


void UTranspoterComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UTranspoterComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UTranspoterComponent::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;
	ArePointsSet = true;
}

