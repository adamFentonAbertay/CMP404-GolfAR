// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/Pawn.h"
#include "PlaceableActor.h"
#include "CustomARPawn.generated.h"

class UCameraComponent;

UCLASS()
class UE5_AR_API ACustomARPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ACustomARPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	
	virtual void OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
	UMaterialInterface* defaultMat;

	UPROPERTY(Category = "CustomARClasses", VisibleAnywhere, BlueprintReadWrite)
	UMaterialInterface* redMat;


	virtual void OnHold();

	virtual void OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos);

	virtual void MoveSelectedActor(FVector2D screenPos);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input

	virtual void checkForActors();


	virtual bool worldHitTest(FVector2D, FHitResult&);

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		USceneComponent* SceneComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* CameraComponent;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		APlaceableActor* SelectedActor;


		UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		APlaceableActor* prevActor;

	UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		bool isFingerDown;

		UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		bool onHolding;

		UPROPERTY(Category = "myCategory", VisibleAnywhere, BlueprintReadWrite)
		float chargeLevel;


};
