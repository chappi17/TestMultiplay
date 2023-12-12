// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Particles/ParticleSystem.h"
#include "MyBox.generated.h"

UCLASS()
class TESTMULTIPLAY_API AMyBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMyBox();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(ReplicatedUsing = OnRep_ReplicatedVar, BlueprintReadWrite)
	float ReplicatedVar;

	UFUNCTION(BluePrintCallable)
	void OnRep_ReplicatedVar();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void DecreaseReplicatedVar();
	FTimerHandle TestTimer;

	UFUNCTION(NetMulticast, Reliable)
	void multicastRPCFunction();

	UPROPERTY(EditAnyWhere)
	UParticleSystem* ExplosionEffect;

};
