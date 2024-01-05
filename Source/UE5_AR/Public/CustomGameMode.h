// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFramework/GameModeBase.h"
#include "HelloARManager.h"
#include "Blueprint/UserWidget.h"
#include "MyButtonUserWidget.h"
#include "MyCustomUserWidget.h"
#include "CustomGameMode.generated.h"

//Forward Declarations
class APlaceableActor;

/**
 * 
 */

UCLASS()
class UE5_AR_API ACustomGameMode : public AGameModeBase
{
	GENERATED_BODY()
private:

	FTimerHandle Ticker;
	APlaceableActor* SpawnedActor;
	APlaceableActor* SpawnedArrow;

public:
	ACustomGameMode();
	virtual ~ACustomGameMode() = default;


	virtual void StartPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Audio")
	void playGolfSFX();
	/**
	 * @brief An example of an event as generally seen within the Event Graph of Blueprints
	 *  You can have different implementations of this event which will be called when you call this as a delegate.
	 *	See the CPP For more information on the implementation
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "GameModeBase", DisplayName = "Start Play")
		void StartPlayEvent();

	UFUNCTION(BlueprintCallable, Category = "Score")
		int32 GetScore();

	UFUNCTION(BlueprintCallable, Category="Score")
		void SetScore(const int32 NewScore);
	/**
	 * @brief A Dummy event for spawning ACustomActor.
	 */
	UFUNCTION(BlueprintCallable, Category="Spawn")
		virtual void SpawnCube();

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	virtual void getCameraForward(FVector camForward);

	

	
	virtual void Tick(float DeltaSeconds) override;

	/**
	 * @brief Perform a line trace at the screen position provided and place a default Placeable actor on it.
	 * Create another actor class that inherits from APlaceableActor and is spawned here instead of the default one. Make sure this new object is placed on the Plane rather than halfway through a plane
	 * @param ScreenPos Pass the Screen Position as an FVector
	 */
	virtual void LineTraceSpawnActor(FVector ScreenPos);

	virtual void ActorMoveForShot(FVector cameraForward, float charge);

	virtual void LineTraceSpawnArrow(FVector ScreenPos);
	

	/**
	 * @brief This function is virtual - you can inherit this class to override this function
	 * Each level can have their own unique spawned actors but with similar base qualities using inheritance
	 */
	virtual void SpawnInitialActors();

	 UPROPERTY(Category="Placeable",EditAnywhere,BlueprintReadWrite)
	 TSubclassOf<APlaceableActor> PlacableToSpawn;

	 UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<APlaceableActor> PlacableToImageTrack;

	 UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<APlaceableActor> MutatorToImageTrack;

	 UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<APlaceableActor> PlacableToSpawnArrow;

	 /*UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UUserWidget> UIShotBar;*/

	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UMyCustomUserWidget> UIWidget;

	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UMyCustomUserWidget> UIWinWidgetSet;

	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 TSubclassOf<UMyButtonUserWidget> UIButtonWidgetSet;


	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 UMyCustomUserWidget* UIWinWidget;

	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 UMyCustomUserWidget* ui;

	 UPROPERTY(Category = "UI Elements", EditAnywhere, BlueprintReadWrite)
	 UMyButtonUserWidget* uiButton;

	 UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 AHelloARManager *ARManager; 

	 UPROPERTY(Category = "Placeable", EditAnywhere, BlueprintReadWrite)
	 FVector publicCamForward;

	
	 UPROPERTY(Category = "Audio", EditAnywhere, BlueprintReadWrite)
	 USoundWave* hitSFX;

	 UPROPERTY(Category = "physics settings", EditAnywhere, BlueprintReadWrite)
	 bool operationStateCopy = false;

	 bool haveWon;
};
