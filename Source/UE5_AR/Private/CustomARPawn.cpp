// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomARPawn.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "ARBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ARPin.h"
#include "PlaceableActor.h"
#include "Components/ProgressBar.h"
#include "Camera/CameraComponent.h"
#include "CustomGameMode.h"


// Sets default values
ACustomARPawn::ACustomARPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	auto redMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Assets/Materials/Red_Mat.Red_Mat"));
	redMat = redMaterial.Object;

	auto defaultMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("/Game/Assets/Materials/Default_Mat.Default_Mat"));
	defaultMat = defaultMaterial.Object;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	
	CameraComponent->SetupAttachment(SceneComponent);

	chargeLevel = 0.f;

	onHolding = false;
}

// Called when the game starts or when spawned
void ACustomARPawn::BeginPlay()
{
	Super::BeginPlay();

	//CameraComponent->GetForwardVector()

}

// Called every frame
void ACustomARPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);
	
	GM->ui->SetVisibility(ESlateVisibility::Hidden);

	
	if (GM->operationStateCopy == false) {
		GM->ARManager->MakePlanesVisible();
	}
	else {

		GM->ARManager->MakePlanesInvisible();
	}

	if (isFingerDown) {
		OnHold();

		TArray<UStaticMeshComponent*> StaticComps;
		if (SelectedActor != NULL) {

			SelectedActor->GetComponents<UStaticMeshComponent>(StaticComps);
		}
		
		if (SelectedActor != NULL && StaticComps[0]->ComponentHasTag("Ball")) {
			if (chargeLevel <= 5) {
				chargeLevel = chargeLevel + (DeltaTime * 10);
			}

			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(chargeLevel), true, true, FLinearColor(1, 0, 1));
			GM->ui->SetVisibility(ESlateVisibility::Visible);
		}
		
	}
	


	


	GM->getCameraForward(CameraComponent->GetForwardVector());
	
	//checkForActors();
	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, GetTransform().ToString());
	GM->ui->setProgVal(chargeLevel);



	//TArray<UObject*> widgetObjects;

	//GM->UIWidget->GetDefaultSubobjects(widgetObjects);

	//

	//GM->UIWidget


	//TArray<UObject*> progressBarList;
	//widgetObjects[0]->GetDefaultSubobjects(progressBarList);
	/*if (SelectedActor) {
		if (SelectedActor->GetVelocity().X <= 0 && SelectedActor->GetVelocity().Y <= 0 && SelectedActor->GetVelocity().Z <= 0) {
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ball no move"));
			TArray<UStaticMeshComponent*> StaticComps;
			SelectedActor->GetComponents<UStaticMeshComponent>(StaticComps);
			StaticComps[0]->SetSimulatePhysics(false);
		}
	}*/

	//FString name = progressBarList[0]->GetFName().ToString();

	//UKismetSystemLibrary::PrintString(this, GM->UIWidget->Children->GetDisplayNameText().ToString(), true, true, FLinearColor(1, 0, 1));

}

// Called to bind functionality to input
void ACustomARPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//Bind various player inputs to functions
	// There are a few types - BindTouch, BindAxis, and BindEvent.  
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACustomARPawn::OnScreenTouch);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACustomARPawn::OnScreenRelease);
}

void ACustomARPawn::OnScreenTouch(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	isFingerDown = true;
	
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	GM->getCameraForward(CameraComponent->GetForwardVector());

	/*TArray<UObject*> widgetObjects;
	
	GM->UIWidget->GetDefaultSubobjects(widgetObjects);*/
//	GM->UIWidget
	//GM->UIWidget->prog
	//GM->UIWidget->ProgBar


	//TArray<UObject*> progressBarList;
	//widgetObjects[0]->GetDefaultSubobjects(progressBarList);



	//FString name = progressBarList[0]->GetFName().ToString();

//	UKismetSystemLibrary::PrintString(this, widgetObjects[0]->GetName(), true, true, FLinearColor(1, 0, 1));


	
	
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ScreenTouch Reached"));
	if(GM)
	{
		// Add call to the line-trace here from the Custom Game Mode
		//GM->LineTraceSpawnActor(ScreenPos);	

		FString displayText = ScreenPos.ToString();

		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, displayText);
	}




	FHitResult ActorHitResult;

	if (worldHitTest(FVector2D(ScreenPos.X, ScreenPos.Y), ActorHitResult))
	{
		//clicked on the circle
		
	//	GM->ARManager->MakePlanesInvisible();
		
		
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("CHARGE SHOT!"));

		// Get object of actor hit.
		UClass* hitActorClass = UGameplayStatics::GetObjectClass(ActorHitResult.GetActor());
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("ActorSearching Reached"));

		if (UKismetMathLibrary::ClassIsChildOf(hitActorClass, APlaceableActor::StaticClass()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, ActorHitResult.GetActor()->GetName());
			
			
			SelectedActor = Cast<APlaceableActor>(ActorHitResult.GetActor());

			//SelectedActor->StaticMeshComponent->SetMaterial(0, redMat);

			//GM->ARManager->

			return;
		}
	}
	else if (SelectedActor != NULL) {
		//->LineTraceSpawnActor(ScreenPos);
	//	GM->ARManager->MakePlanesVisible();

		//SelectedActor->StaticMeshComponent->SetMaterial(0, defaultMat);

		SelectedActor = NULL;
		//set color
	}

	else {
		GM->LineTraceSpawnActor(ScreenPos);
		if (GM->operationStateCopy == false) {
			GM->LineTraceSpawnArrow(ScreenPos);
		}
		
	}


}

void ACustomARPawn::OnHold()
{
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);

	TArray<UStaticMeshComponent*> StaticComps;
	if (SelectedActor != NULL) {
		
		SelectedActor->GetComponents<UStaticMeshComponent>(StaticComps);
	}

	if (SelectedActor == NULL || StaticComps[0]->ComponentHasTag("Ball") == false) { return; };

	//if (SelectedActor->GetName() != GM->ballName) { return;  }

	SelectedActor->isShot = false;
	onHolding = true;
	

	GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Green, TEXT("On Hold Reached"));

	

	// Finger X and Y position
	float fingerX;
	float fingerY;

	bool isTouched;    // Needed for the input touch state funtion

	GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, fingerX, fingerY, isTouched);

	//MoveSelectedActor(FVector2D(fingerX, fingerY));
}

void ACustomARPawn::OnScreenRelease(const ETouchIndex::Type FingerIndex, const FVector ScreenPos)
{
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Blue, TEXT("On Release Reached"));
	isFingerDown = false;
	
	auto Temp = GetWorld()->GetAuthGameMode();
	auto GM = Cast<ACustomGameMode>(Temp);


	if (onHolding) {
			UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(chargeLevel), true, true, FLinearColor(1, 0, 1));
			
			
			//THIS
			//SelectedActor->isShot = true;




			/*
			* solution 1
			FVector curLoc = SelectedActor->GetActorLocation();
			FVector newLoc = curLoc;
			newLoc = newLoc * (SelectedActor->GetActorForwardVector() * chargeLevel);
			SelectedActor->SetActorLocation(newLoc);
			SelectedActor->GetActorTransform().GetLocation();*/
			
			//-0.57, -0.2, -0.79

			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->GetActorLocation().X), true, true, FLinearColor(1, 0, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->GetActorLocation().Y), true, true, FLinearColor(1, 0, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->GetActorLocation().Z), true, true, FLinearColor(1, 0, 1));


			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->PinComponent->GetLocalToWorldTransform().GetLocation().X), true, true, FLinearColor(0, 1, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->PinComponent->GetLocalToWorldTransform().GetLocation().Y), true, true, FLinearColor(0, 1, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(SelectedActor->PinComponent->GetLocalToWorldTransform().GetLocation().Z), true, true, FLinearColor(0, 1, 1));
		
			////-102, -46.1, -192
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(CameraComponent->GetForwardVector().X), true, true, FLinearColor(1, 0, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(CameraComponent->GetForwardVector().Y), true, true, FLinearColor(1, 0, 1));
			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(CameraComponent->GetForwardVector().Z), true, true, FLinearColor(1, 0, 1));

			
			
			//solution 2
			/*FVector Translation = CameraComponent->GetForwardVector() * chargeLevel;
			FTransform moveTransform;
			moveTransform.SetTranslation(Translation);
			SelectedActor->AddActorLocalTransform(moveTransform);*/

			// try matrix multiplkcation
			//solution 3

			//GM->ActorMoveForShot(CameraComponent->GetForwardVector() , chargeLevel);

			//neither statement running
		/*	if (prevActor != NULL) {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("prev actor valid"));
				UPrimitiveComponent* primitive = prevActor->FindComponentByClass<UPrimitiveComponent>();
				if (primitive != NULL) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("prev actors primitive valid"));
					primitive->AddForce(CameraComponent->GetForwardVector() * 1000 * chargeLevel);
				}
				else {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("prev actor primitive invalid"));
				}
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("prev actor invalid"));
			}*/


			if (SelectedActor != NULL) {
		

				//UKismetSystemLibrary::PrintString(this, SelectedActor->get().ToString(), true, true, FLinearColor(1, 0, 1));
				
				//i reckon pin becomes null s crashes
				auto TrackingState = SelectedActor->PinComponent->GetTrackingState();

				//TrackingState = EARTrackingState::NotTracking;


				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("selected actor valid"));
				
			//	UPrimitiveComponent* primitive2 = SelectedActor->FindComponentByClass<UPrimitiveComponent>();
			/*	if (primitive2 != NULL) {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("selected actors primitive valid"));
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Magenta, TEXT("SHOT GO APPLYIG FORCE"));


					primitive2->AddForce(CameraComponent->GetForwardVector() * 1 * chargeLevel);

					FTransform a = SelectedActor->GetTransform();
					FVector newVec = a.GetLocation();
					newVec = newVec * CameraComponent->GetForwardVector() * chargeLevel;
					a.TransformPosition(newVec);
					a.SetLocation


					SelectedActor->SetActorLocation(newVec);
					
				
				}*/
			/*	else {
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("selected actor primitive invalid"));
				}*/
			}
			else {
				GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("selected actor invalid"));
			}


			//selected actor null?

			FVector vec1 = SelectedActor->GetActorForwardVector();

			vec1.Z = 0;


			/*float lenSq1 = (vec1.X * vec1.X) + (vec1.Y * vec1.Y) + (vec1.Z * vec1.Z);
			float lenSq2 = (camForward.X * camForward.X) + (camForward.Y * camForward.Y) + (camForward.Z * camForward.Z);*/

			float dot = FVector::DotProduct(vec1, CameraComponent->GetForwardVector());
			float det = (vec1.X * CameraComponent->GetForwardVector().Y) - (vec1.Y * CameraComponent->GetForwardVector().X);

			double newAngle = atan2(det, dot);


			/* newAngle = acosf(FVector::DotProduct(vec1, camForward) / sqrt(lenSq1 + lenSq2)); */

			newAngle = FMath::RadiansToDegrees(newAngle);

			

			APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
			FVector camLocation = camManager->GetCameraLocation();
			FVector camForward = camManager->GetCameraRotation().Vector();


			camForward.Z = 0;
			//SelectedActor->SetActorLocation(SelectedActor->GetActorLocation() + (camForward * chargeLevel));
			TArray<UStaticMeshComponent*> StaticComps;
			SelectedActor->GetComponents<UStaticMeshComponent>(StaticComps);
			//StaticComps[0]->SetSimulatePhysics(true);
			//WHEN OPEN NEXT TIME, TRY START DISABLED AND MAKE ENABLED
			
			
			StaticComps[0]->AddImpulse(camForward * (chargeLevel * 2));

			GM->playGolfSFX();
			//THIS WORKS TP
			/*
				const FRotator MoveRotation(0.f, SelectedActor->GetActorRotation().Yaw, 0.f);
				const FVector MoveDirection = FRotationMatrix(MoveRotation).GetScaledAxis(EAxis::X);

				SelectedActor->AddActorLocalOffset(-MoveDirection);*/

			
			
			
			
			
			
			
			//SelectedActor->GetComponentsByClass<UStaticMeshComponent>()->AddForce(camForward * chargeLevel);

			
			
			

			//needs acos2f as only 180 of rot, look 1st year proj

			//FRotator newRot = FRotator(0, FMath::RadiansToDegrees(newAngle) - SpawnedArrow->GetActorRotation().Yaw, 0);

			FRotator newRot = FRotator(0, newAngle + SelectedActor->GetActorRotation().Yaw, 0);

			//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(newAngle), true, true, FLinearColor(1,0,1));

			//SpawnedArrow->SetActorRelativeRotation(newRot);
			SelectedActor->SetActorRotation(newRot);

			//THIS WORKS
		/*	float x = cos(newAngle + SelectedActor->GetActorRotation().Yaw) * cos(0);
			float y = sin(newAngle + SelectedActor->GetActorRotation().Yaw) * cos(0);
			float z = sin(0);


			UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(x), true, true, FLinearColor(1, 0, 0));
			UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(y), true, true, FLinearColor(1, 0, 0));
			UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(z), true, true, FLinearColor(1, 0, 0));

			FVector newVec;
			newVec.X = x;
			newVec.Y = y;
			newVec.Z = z;*/

			//SelectedActor->SetActorLocation(SelectedActor->GetActorLocation() + (newVec));

		// 	SelectedActor->SetActorLocation(SelectedActor->GetActorLocation() + (CameraComponent->GetForwardVector().Normalize()* chargeLevel));
			


			


			chargeLevel = 0;
		onHolding = false;
	}
	else {

	}
}

bool ACustomARPawn::worldHitTest(FVector2D screenPos, FHitResult& hitResult) {

	// Get player controller
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);

	// Perform deprojection taking 2d clicked area and generating reference in 3d world-space.
	FVector worldPosition;
	FVector worldDirection; // Unit Vector

	bool deprojectionSuccess = UGameplayStatics::DeprojectScreenToWorld(playerController, screenPos, /*out*/ worldPosition, /*out*/ worldDirection);

	// construct trace vector (from point clicked to 1000.0 units beyond in same direction)
	FVector traceEndVector = worldDirection * 1000.0;
	traceEndVector = worldPosition + traceEndVector;

	// perform line trace (Raycast)
	bool traceSuccess = GetWorld()->LineTraceSingleByChannel(hitResult, worldPosition, traceEndVector, ECollisionChannel::ECC_WorldDynamic);

	// return if the operation was successful
	return traceSuccess;
}

void ACustomARPawn::MoveSelectedActor(FVector2D screenPoser)
{
	float fingerX;
	float fingerY;

	bool isTouched;

	GetWorld()->GetFirstPlayerController()->GetInputTouchState(ETouchIndex::Touch1, fingerX, fingerY, isTouched);

	FVector2D screenPos = FVector2D(fingerX, fingerY);


	//Basic variables for functionality
	APlayerController* playerController = UGameplayStatics::GetPlayerController(this, 0);
	FVector WorldPos;
	FVector WorldDir;

	//Gets the screen touch in world space and the tracked objects from a line trace from the touch
	UGameplayStatics::DeprojectScreenToWorld(playerController, FVector2D(screenPos), WorldPos, WorldDir);
	// Notice that this LineTrace is in the ARBluePrintLibrary - this means that it's exclusive only for objects tracked by ARKit/ARCore
	auto TraceResult = UARBlueprintLibrary::LineTraceTrackedObjects(FVector2D(screenPos), false, false, false, true);

	//Checks if the location is valid
	if (TraceResult.IsValidIndex(0))
	{
		// Get the first found object in the line trace - ignoring the rest of the array elements
		auto TrackedTF = TraceResult[0].GetLocalToWorldTransform();
		//
		SelectedActor->SetActorLocation(TrackedTF.GetLocation());

		if (FVector::DotProduct(TrackedTF.GetRotation().GetUpVector(), WorldDir) < 0)
		{
			//Spawn the actor pin and get the transform
			UARPin* ActorPin = UARBlueprintLibrary::PinComponent(nullptr, TraceResult[0].GetLocalToWorldTransform(), TraceResult[0].GetTrackedGeometry());

			// Check if ARPins are available on your current device. ARPins are currently not supported locally by ARKit, so on iOS, this `ll always be "FALSE" 
			if (ActorPin)
			{
				// Set the  actor location based on the Pin. Have a look at the code for Placeable Object to see how it handles the AR PIN passed on
				SelectedActor->SetActorLocation(TrackedTF.GetLocation());
				SelectedActor->PinComponent = ActorPin;
			}

		}
	}
}


void ACustomARPawn::checkForActors() {
	TArray<AActor*, FDefaultAllocator> PlacedActors;

	// Find placed actors
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlaceableActor::StaticClass(), PlacedActors);

	// Get camera loaction
	APlayerCameraManager* camManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	FVector camLocation = camManager->GetCameraLocation();

	// Initialise FVector for actor location
	FVector actorLocation;

	// Distance to the actor for material switch
	int16 distToActor = 100;

	// Loop through each found actor
	for (int i = 0; i < PlacedActors.Num(); i++) {
		// Current actor location
		actorLocation = PlacedActors[i]->GetActorLocation();

		// Find whether actor is within range
		/*if (FVector::Distance(camLocation, actorLocation) < distToActor)
		{
			Cast<APlaceableActor>(PlacedActors[i])->StaticMeshComponent->SetMaterial(0, redMat);
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, TEXT("Actor Within Range"));
		}
		else
		{
			Cast<APlaceableActor>(PlacedActors[i])->StaticMeshComponent->SetMaterial(0, defaultMat);
			GEngine->AddOnScreenDebugMessage(-1, 0.01f, FColor::Yellow, TEXT("Actor Outside Range"));
		}*/

		// Reset the location
		actorLocation = FVector::ZeroVector;
	}
}
