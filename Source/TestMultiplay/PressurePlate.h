// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "PressurePlate.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnDeActivated);

UCLASS()
class TESTMULTIPLAY_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	APressurePlate();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* TriggerMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool Activated;

	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnActivated OnActivatedDel;

	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnDeActivated OnDeActivatedDel;
};
