// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TranspoterComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TESTMULTIPLAY_API UTranspoterComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UTranspoterComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool ArePointsSet; 

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;
	
	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;
	
	UPROPERTY(BlueprintReadWrite,VisibleAnywhere)
	bool AllTriggerActorsTriggered;

	UFUNCTION()
	void SetPoints(FVector Point1, FVector Point2);
};
