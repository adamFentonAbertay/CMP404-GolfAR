// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BasicPlacableBase.generated.h"

UCLASS()
class UE5_AR_API ABasicPlacableBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABasicPlacableBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	//UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
	//USceneComponent* SceneComponent;

	//UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
	//UStaticMeshComponent* StaticMeshComponent;

	//UARPin* PinComponent;

};
