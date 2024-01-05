// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameMode.h"
#include "CustomARPawn.h"
#include "CustomGameState.h"
#include "CustomActor.h"
#include "HelloARManager.h"
#include "ARPin.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "PlaceableActor.h"


ACustomGameMode::ACustomGameMode():SpawnedActor(nullptr)

{
	// Add this line to your code if you wish to use the Tick() function
	PrimaryActorTick.bCanEverTick = true;

	// Set the default pawn and gamestate to be our custom pawn and gamestate programatically
	DefaultPawnClass = ACustomARPawn::StaticClass();
	GameStateClass = ACustomGameState::StaticClass();
	haveWon = false;
	
}


void ACustomGameMode::StartPlay() 
{
	operationStateCopy = false;

	//setting ui elements from blueprint and creating them as widgets to display
	if (UIWidget)
	{
		ui = CreateWidget<UMyCustomUserWidget>(GetWorld(), UIWidget);
		ui->AddToViewport();
		//ui->setProgVal(.40);
	}

	if (UIWinWidgetSet) {
		UIWinWidget = CreateWidget<UMyCustomUserWidget>(GetWorld(), UIWinWidgetSet);
		UIWinWidget->AddToViewport();
		UIWinWidget->SetVisibility(ESlateVisibility::Hidden);
	}

	if (UIButtonWidgetSet) {
		uiButton = CreateWidget<UMyButtonUserWidget>(GetWorld(), UIButtonWidgetSet);
		uiButton->AddToViewport();
		
	}
	
	SpawnInitialActors();

	// This is called before BeginPlay
	StartPlayEvent();
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("Shots taken: %d"), GetScore()));

	// This function will transcend to call BeginPlay on all the actors 
	Super::StartPlay();
	
}

// An implementation of the StartPlayEvent which can be triggered by calling StartPlayEvent() 
void ACustomGameMode::StartPlayEvent_Implementation() 
{
	// Start a timer which will call the SpawnCube Function every 4 seconds
	//GetWorldTimerManager().SetTimer(Ticker, this, &ACustomGameMode::SpawnCube, 4.0f, true, 0.0f);
}

int32 ACustomGameMode::GetScore()
{
	return GetGameState<ACustomGameState>()->Score;
}

void ACustomGameMode::SetScore(const int32 NewScore)
{
	GetGameState<ACustomGameState>()->Score = NewScore;

}

void ACustomGameMode::SpawnCube()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(-300, 0, 0);
	ACustomActor* CustomActor = GetWorld()->SpawnActor<ACustomActor>(MyLoc, MyRot, SpawnInfo);


}

void ACustomGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (uiButton) {
		operationStateCopy = uiButton->operationState;
	}

	//if (operationStateCopy == true && SpawnedArrow) {
	//	SpawnedArrow->SetActorLocation(SpawnedActor->GetActorLocation());
	//}
	// 
	//to stop physics

	/*if (uiButton) {
		
		if (uiButton->getButtonstate()) {
			
			if (SpawnedActor) {
				TArray<UStaticMeshComponent*> StaticComps;
				SpawnedActor->GetComponents<US
				taticMeshComponent>(StaticComps);
				StaticComps[0]->SetSimulatePhysics(false);
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("button press"));
			}
		}
	}*/

	if (uiButton) {
		if (uiButton->operationState == true) {
			if (SpawnedActor) {
				TArray<UStaticMeshComponent*> StaticComps;
				SpawnedActor->GetComponents<UStaticMeshComponent>(StaticComps);
				StaticComps[0]->SetSimulatePhysics(true);
			}
		}
		else {
			if (SpawnedActor) {
				TArray<UStaticMeshComponent*> StaticComps;
				SpawnedActor->GetComponents<UStaticMeshComponent>(StaticComps);
				StaticComps[0]->SetSimulatePhysics(false);
			}
		}
	}
	

	//failsafe
	/*if (SpawnedArrow) {
		const FRotator MoveRotation(0.f, SpawnedArrow->GetActorRotation().Yaw, 0.f);
		const FVector MoveDirection = FRotationMatrix(MoveRotation).GetScaledAxis(EAxis::X);

		SpawnedActor->AddActorLocalOffset(-MoveDirection);
	}*/

	//win condition (is ball close enough to flag? then display)
	if (SpawnedActor) {

		
		TArray<UStaticMeshComponent*> StaticComps;
		SpawnedActor->GetComponents<UStaticMeshComponent>(StaticComps);
		//StaticComps[0]->SetSimulatePhysics(true);
		//WHEN OPEN NEXT TIME, TRY START DISABLED AND MAKE ENABLED

	/*	UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(abs(StaticComps[0]->GetRelativeLocation().X)), true, true, FLinearColor(1, 0, 1));
		UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(abs(ARManager->getFlagLoc().X)), true, true, FLinearColor(1, 0, 1));*/



		//failsafe checking if necessary game flags are triggered and checking relatives position to see if ball in hole
		if ( ARManager->bGoghFound == true) {
			if (abs(abs(StaticComps[0]->GetRelativeLocation().X) - abs(ARManager->getFlagLoc().X)) <= 2.5f) {
				if (abs(abs(StaticComps[0]->GetRelativeLocation().Y) - abs(ARManager->getFlagLoc().Y)) <= 2.5f) {
					if (haveWon == false) {
						UIWinWidget->SetVisibility(ESlateVisibility::Visible);
					}

				}
			}
		}

		//failsafe checking if necessary game flags are triggered and checking relatives position to see if ball in hole

		//if StaticComps[0]->GetRelativeLocation().X >= -1.5f &&  <= 1.5f
	/*	if (ARManager->bGoghFound == true) {
			if (StaticComps[0]->GetRelativeLocation().X - ARManager->getFlagLoc().X >= -1.5f && StaticComps[0]->GetRelativeLocation().X - ARManager->getFlagLoc().X <= 1.5f) {
				if (StaticComps[0]->GetRelativeLocation().Y - ARManager->getFlagLoc().Y >= -1.5f && StaticComps[0]->GetRelativeLocation().Y - ARManager->getFlagLoc().Y <= 1.5f) {
					if (haveWon == false) {
						UIWinWidget->SetVisibility(ESlateVisibility::Visible);
					}

				}
			}
		}*/
	}
}

void ACustomGameMode::SpawnInitialActors()
{

	// Spawn an instance of the HelloARManager class
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(-300, 0, 0);


	ARManager = GetWorld()->SpawnActor<AHelloARManager>(MyLoc, MyRot, SpawnInfo);
	ARManager->SetObjectToSpawn(PlacableToImageTrack, MutatorToImageTrack);
	
}

void ACustomGameMode::getCameraForward(FVector camForward) {
	if (SpawnedArrow) {
		
		//ignore Z as arrow should just display yaw
		camForward.Z = 0;
		camForward.Normalize();
		
		//double newAngle = acosf(FVector::DotProduct(SpawnedArrow->GetActorForwardVector(), camForward));

		FVector vec1 = SpawnedArrow->GetActorForwardVector();
		vec1.Z = 0;


		//dot product and determinant between camera and current arrows direction
		float dot = FVector::DotProduct(vec1, camForward);
		float det = (vec1.X * camForward.Y) - (vec1.Y * camForward.X);

		//new angle to rotate to
		double newAngle = atan2(det, dot);

		/* newAngle = acosf(FVector::DotProduct(vec1, camForward) / sqrt(lenSq1 + lenSq2)); */

		newAngle = FMath::RadiansToDegrees(newAngle);


		//needs acos2f as only 180 of rot, look 1st year proj - DONE
		
		//rotate to new pos
		FRotator newRot = FRotator(0, newAngle + SpawnedArrow->GetActorRotation().Yaw + 90, 0) ;
		

		SpawnedArrow->SetActorRotation(newRot);
		SpawnedActor->SetActorRotation(newRot);

		
	}
}

void ACustomGameMode::playGolfSFX() {
	UGameplayStatics::PlaySound2D(GetWorld(), hitSFX);
}
//FUNCTION REDACTED - left in as a failsafe but this function allows for object movement with shot power without the use of phyiscs.
void ACustomGameMode::ActorMoveForShot(FVector cameraForward, float charge)
{

	//no commenting -- see above

	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	

	//Checks if the location is valid
	
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = SpawnedActor->GetTransform();

		

		
		//old code for how much ball should move
		TrackedTF.SetTranslation(TrackedTF.GetTranslation() * (cameraForward * charge * 2));

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TrackedTF);

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			if (ActorPin)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ARPin is valid"));
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedActor)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);

					const FVector MyLoc(0, 0, 0);
					//SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(MyLoc, MyRot, SpawnInfo);
					SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
					SpawnedActor->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));

				}

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: setting transform and scale "));
				// Set the spawned actor location based on the Pin. Have a look at the code for Placeable Object to see how it handles the AR PIN passed on
				SpawnedActor->SetActorTransform(PinTF);
				SpawnedActor->PinComponent = ActorPin;

				SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));
				//GetWorld()->camera
			}
			// IF ARPins are Not supported locally (for iOS Devices) We will spawn the object in the location where the line trace has hit
			else {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ARPin is invalid"));
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedActor)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);
					const FVector MyLoc(0, 0, 0);
					SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
					SpawnedActor->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
				}
				//stopping overpowering when shot
				if (SpawnedActor->isShot == true) {
					SpawnedActor->SetActorTransform(TrackedTF);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: setting transform and scale"));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: setting transform and scale not shot"));
				}
				SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));
				
			}
		}
	

}

void ACustomGameMode::LineTraceSpawnActor(FVector ScreenPos)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Line Trace Reached"));

	//SpawnCube();

	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;
	
	//playerController->GetInputMotionState()
	//playerController->getcamera
		

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);

	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			if (ActorPin)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ARPin is valid"));
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedActor)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);
				
					const FVector MyLoc(0, 0, 0);
					//SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(MyLoc, MyRot, SpawnInfo);
					SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
					SpawnedActor->SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));

					

					//StaticComps[0]->SetSimulatePhysics(false);
					SpawnedActor->SetActorTransform(PinTF);
				
				}
				
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: setting transform and scale "));
				// Set the spawned actor location based on the Pin. Have a look at the code for Placeable Object to see how it handles the AR PIN passed on
				SpawnedActor->SetActorTransform(PinTF);
				SpawnedActor->SetActorLocation(PinTF.GetLocation());
				SpawnedActor->PinComponent = ActorPin;


				//maybe delete
				SpawnedActor->SetActorTransform(TrackedTF);
			
				SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));
				//GetWorld()->camera
			}
			// IF ARPins are Not supported locally (for iOS Devices) We will spawn the object in the location where the line trace has hit
			else {
			
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ARPin is invalid"));
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedActor)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);
					const FVector MyLoc(0, 0, 0);
					SpawnedActor = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawn, MyLoc, MyRot, SpawnInfo);
					SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));
				}
				//stopping overpowering when shot
				if (SpawnedActor->isShot == true) {
					SpawnedActor->SetActorTransform(TrackedTF);
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("TRACKEDTF"));
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("TRACKTEDTFELSE"));
				}
				SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));

			
			}
			
		}
		SpawnedActor->SetActorScale3D(FVector(0.1, 0.1, 0.1));
	}


	//for (UActorComponent* ActorComponent : SpawnedActor->GetComponents())
	//{
	//	// Check if the component is a UStaticMeshComponent
	//	UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(ActorComponent);

	//	if (StaticMeshComponent)
	//	{
	//		// Disable physics temporarily
	//		StaticMeshComponent->SetSimulatePhysics(false);

	//		// Set the actor's location
	//		/*FVector DesiredLocation = YourDesiredLocation;
	//		SetActorLocation(DesiredLocation);*/

	//		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("component found"));

	//		// Re-enable physics
	//		StaticMeshComponent->SetSimulatePhysics(true);

	//		// Break out of the loop since you found the UStaticMeshComponent
	//		break;
	//	}
	//}
}

void ACustomGameMode::LineTraceSpawnArrow(FVector ScreenPos)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("Line Trace Reached"));

	//SpawnCube();

	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(ScreenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(ScreenPos), false, false, false, true);

	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this will always be "FALSE" 
			if (ActorPin)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ARPin is valid"));
				//If the pin is valid 
				auto PinTF = ActorPin->GetLocalToWorldTransform();
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedArrow)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);
					const FVector MyLoc(0, 0, 0);
					SpawnedArrow = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawnArrow, MyLoc, MyRot, SpawnInfo);
					SpawnedArrow->SetActorScale3D(FVector(0.6f, 0.6f, 0.2f));
				}

				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("arrow: setting transform and scale"));
				// Set the spawned actor location based on the Pin. Have a look at the code for Placeable Object to see how it handles the AR PIN passed on
				SpawnedArrow->SetActorTransform(PinTF);
				SpawnedArrow->PinComponent = ActorPin;
				SpawnedArrow->SetActorScale3D(FVector(0.6f, 0.6f, 0.2f));

			}
			// IF ARPins are Not supported locally (for iOS Devices) We will spawn the object in the location where the line trace has hit
			else {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("ARPin is invalid"));
				//Spawn a new Actor at the location if not done yet
				if (!SpawnedArrow)
				{
					const FActorSpawnParameters SpawnInfo;
					const FRotator MyRot(0, 0, 0);
					const FVector MyLoc(0, 0, 0);
					SpawnedArrow = GetWorld()->SpawnActor<APlaceableActor>(PlacableToSpawnArrow, MyLoc, MyRot, SpawnInfo);
					SpawnedArrow->SetActorScale3D(FVector(0.6f, 0.6f, 0.2f));
				}
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("arrow: setting transform and scale2"));
				SpawnedArrow->SetActorTransform(TrackedTF);
				SpawnedArrow->SetActorScale3D(FVector(0.6f, 0.6f, 0.2f));


			}
		}
	}
}


