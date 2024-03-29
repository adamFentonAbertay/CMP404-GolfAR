// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "PlaceableActor.h"
#include "HelloARManager.generated.h"

class UARSessionConfig;
class AARPlaneActor;
class UARPlaneGeometry;

UCLASS()
class UE5_AR_API AHelloARManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHelloARManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
	UMaterialInterface* blankMat;

public:	

	bool bGoghFound;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void MakePlanesInvisible();

	virtual void SetObjectToSpawn(TSubclassOf <APlaceableActor>, TSubclassOf <APlaceableActor>);

	virtual void MakePlanesVisible();

	FVector getFlagLoc();

	//Create a default Scene Component
	UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

		UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		APlaceableActor *GoghCube;

		UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		APlaceableActor* MutObj;

		UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf <APlaceableActor> spawnableActor;

		UPROPERTY(Category = "SceneComp", VisibleAnywhere, BlueprintReadWrite)
		TSubclassOf <APlaceableActor> mutatorActor;

protected:
	
	// Updates the plane actors on every frame as long as the AR Session is running
	void UpdatePlaneActors();

	

	AARPlaneActor* SpawnPlaneActor();
	FColor GetPlaneColor(int Index);
	void ResetARCoreSession();
	// Configuration file for AR Session
	UARSessionConfig* Config;

	//Base plane actor for geometry detection
	AARPlaneActor* PlaneActor;


	//Map of geometry planes
	TMap<UARPlaneGeometry*, AARPlaneActor*> PlaneActors;

	//APlaceableActor* GoghCube;
	

	
	bool bMutFound;

	//Index for plane colours adn array of colours
	int PlaneIndex = 0;
	TArray<FColor> PlaneColors;

};
