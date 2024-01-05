// Fill out your copyright notice in the Description page of Project Settings.


#include "HelloARManager.h"
#include "ARPlaneActor.h"
#include "ARPin.h"
#include "ARSessionConfig.h"
#include "ARBlueprintLibrary.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

// Sets default values
AHelloARManager::AHelloARManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This constructor helper is useful for a quick reference within UnrealEngine and if you're working alone. But if you're working in a team, this could be messy.
	// If the artist chooses to change the location of an art asset, this will throw errors and break the game.
	// Instead let unreal deal with this. Usually avoid this method of referencing.
	// For long term games, create a Data-Only blueprint (A blueprint without any script in it) and set references to the object using the blueprint editor.
	// This way, unreal will notify your artist if the asset is being used and what can be used to replace it.
	static ConstructorHelpers::FObjectFinder<UARSessionConfig> ConfigAsset(TEXT("ARSessionConfig'/Game/Blueprints/HelloARSessionConfig.HelloARSessionConfig'"));
	Config = ConfigAsset.Object;


	auto blankMaterial = ConstructorHelpers::FObjectFinder<UMaterial>(TEXT("Material'/Game/Assets/Materials/ARPlane_Mat.ARPlane_Mat'"));
	blankMat = blankMaterial.Object;

	
	//auto GoghCubeObj = ConstructorHelpers::FObjectFinder<APlaceableActor>(TEXT("PlacableActor'/Game/placeableVanGogh.placeableVanGogh'"));
	//GoghCube = GoghCubeObj.Object;



	//	Blueprint'/Game/placeableVanGogh.placeableVanGogh'


	/*if (GoghCubeObj.Succeeded())
	{
		GoghCube = Cast<APlaceableActor>(GoghCubeObj.Object->GeneratedClass);
		if (GoghCube)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("GohCube Spawned or something idk actually"));
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("GohCube NOT spawned or something idk actually"));
		}
	}*/
	


	//Populate the plane colours array
	PlaneColors.Add(FColor::Blue);
	PlaneColors.Add(FColor::Red);
	PlaneColors.Add(FColor::Green);
	PlaneColors.Add(FColor::Cyan);
	PlaneColors.Add(FColor::Magenta);
	PlaneColors.Add(FColor::Emerald);
	PlaneColors.Add(FColor::Orange);
	PlaneColors.Add(FColor::Purple);
	PlaneColors.Add(FColor::Turquoise);
	PlaneColors.Add(FColor::White);
	PlaneColors.Add(FColor::Yellow);
}

// Called when the game starts or when spawned
void AHelloARManager::BeginPlay()
{
	Super::BeginPlay();
	
	//Start the AR Session
	UARBlueprintLibrary::StartARSession(Config);
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("AR Manager Online"));
	
}

void AHelloARManager::SetObjectToSpawn(TSubclassOf <APlaceableActor> actorToSpawn, TSubclassOf <APlaceableActor> mutatorToSpawn) {
	spawnableActor = actorToSpawn;
	mutatorActor = mutatorToSpawn;
}

// Called every frame
void AHelloARManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<UARTrackedImage*> trackedImages = UARBlueprintLibrary::GetAllTrackedImages();

	//loop through ar managers bp arrau of tracked images
	for (auto& trackedImage : trackedImages) {
		if (trackedImage->GetDetectedImage()) {
			if (trackedImage->GetDetectedImage()->GetFriendlyName().Equals("IDImage"))
			{
				//first detection if true
				if (bGoghFound == false) {

					const FActorSpawnParameters SpawnInfo;
					
					const FRotator MyRot(0, 0, 0);

					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("IDImage"));
					
					//->GetLocalToWorldTransform().GetScale3D();

				//	GoghCube->SetActorLocation(trackedImage->GetLocalToWorldTransform().GetLocation());
					GoghCube = GetWorld()->SpawnActor<APlaceableActor>(spawnableActor, trackedImage->GetLocalToWorldTransform().GetLocation(), MyRot, SpawnInfo);

					GoghCube->SetActorTransform(trackedImage->GetLocalToWorldTransform());

					

					GoghCube->SetActorScale3D(FVector(.05, .05, .05));

					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, trackedImage->GetLocalToWorldTransform().GetScale3D().ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, trackedImage->GetLocalToWorldTransform().GetLocation().ToString());
					bGoghFound = true;
				}
				else {
					const FRotator MyRot(0, 0, 0);
					GoghCube->SetActorRotation(MyRot);
					GoghCube->SetActorLocation(trackedImage->GetLocalToWorldTransform().GetLocation());
				}
			}
			if (trackedImage->GetDetectedImage()->GetFriendlyName().Equals("MutID"))
			{
				if (bMutFound == false) {

					const FActorSpawnParameters SpawnInfo;

					const FRotator MyRot(0, 0, 0);

				

					//->GetLocalToWorldTransform().GetScale3D();

				//	GoghCube->SetActorLocation(trackedImage->GetLocalToWorldTransform().GetLocation());
					MutObj = GetWorld()->SpawnActor<APlaceableActor>(mutatorActor, trackedImage->GetLocalToWorldTransform().GetLocation(), MyRot, SpawnInfo);

					MutObj->SetActorTransform(trackedImage->GetLocalToWorldTransform());



					MutObj->SetActorScale3D(FVector(.05, .05, .05));

					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, trackedImage->GetLocalToWorldTransform().GetScale3D().ToString());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, trackedImage->GetLocalToWorldTransform().GetLocation().ToString());
					bMutFound = true;
				}
				else {
					const FRotator MyRot(0, 0, 0);
					MutObj->SetActorRotation(MyRot);
					MutObj->SetActorLocation(trackedImage->GetLocalToWorldTransform().GetLocation());
				}
			}
		}
	}

	

	switch (UARBlueprintLibrary::GetARSessionStatus().Status)
	{
	case EARSessionStatus::Running:
		
		UpdatePlaneActors();
		break;

	case EARSessionStatus::FatalError:

		ResetARCoreSession();
		UARBlueprintLibrary::StartARSession(Config);
		break;
	}
}



//Updates the geometry actors in the world
void AHelloARManager::UpdatePlaneActors()
{
	//Get all world geometries and store in an array
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	bool bFound = false;

	//Loop through all geometries
	for (auto& It : Geometries)
	{
		//Check if current plane exists 
		if (PlaneActors.Contains(It))
		{
			AARPlaneActor* CurrentPActor = *PlaneActors.Find(It);

			//Check if plane is subsumed
			if (It->GetSubsumedBy()->IsValidLowLevel())
			{
				CurrentPActor->Destroy();
				PlaneActors.Remove(It);
				break;
			}
			else
			{
				//Get tracking state switch
				switch (It->GetTrackingState())
				{
					//If tracking update
				case EARTrackingState::Tracking:
					CurrentPActor->UpdatePlanePolygonMesh();
					break;
					//If not tracking destroy the actor and remove from map of actors
				case EARTrackingState::StoppedTracking:
					CurrentPActor->Destroy();
					PlaneActors.Remove(It);
					break;
				}
			}
		}
		else
		{
			//Get tracking state switch
			switch (It->GetTrackingState())
			{

			case EARTrackingState::Tracking:
				if (!It->GetSubsumedBy()->IsValidLowLevel())
				{
					PlaneActor = SpawnPlaneActor();
					PlaneActor->SetColor(GetPlaneColor(PlaneIndex));
					PlaneActor->ARCorePlaneObject = It;

					PlaneActors.Add(It, PlaneActor);
					PlaneActor->UpdatePlanePolygonMesh();
					PlaneIndex++;
				}
				break;
			}
		}

	}
}

FVector AHelloARManager::getFlagLoc() {
	//return flags location if found, if not just return 0
	if (bGoghFound == true) {
		return GoghCube->GetActorLocation();
	}
	else {
		return FVector(0, 0, 0);
	}
}

void AHelloARManager::MakePlanesInvisible()
{
	//Get all world geometries and store in an array
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	bool bFound = false;

	//Loop through all geometries
	for (auto& It : Geometries)
	{
		//Check if current plane exists 
		if (PlaneActors.Contains(It))
		{
			AARPlaneActor* CurrentPActor = *PlaneActors.Find(It);

			UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(blankMat, NULL);

		/*	FColor invisible = FColor(0, 0, 0, 0);*/
			//Check if plane is subsumed
		//	CurrentPActor->SetColor(invisible);
			CurrentPActor->Material_ = blankMat;
			CurrentPActor->PlaneMaterial = dynamicMaterial;
			CurrentPActor->SetHidden(true);
		
		}

	}
}

void AHelloARManager::MakePlanesVisible()
{
	//Get all world geometries and store in an array
	auto Geometries = UARBlueprintLibrary::GetAllGeometriesByClass<UARPlaneGeometry>();
	bool bFound = false;

	//Loop through all geometries
	for (auto& It : Geometries)
	{
		//Check if current plane exists 
		if (PlaneActors.Contains(It))
		{
			AARPlaneActor* CurrentPActor = *PlaneActors.Find(It);
			//redacted
			UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(blankMat, NULL);

			/*FColor visible;

			visible.MakeRandomColor();*/
			//visible.Transparent;
			CurrentPActor->Material_ = blankMat;
			CurrentPActor->PlaneMaterial = dynamicMaterial;
		
			CurrentPActor->SetHidden(false);

		}

	}
}

//Simple spawn function for the tracked AR planes
AARPlaneActor* AHelloARManager::SpawnPlaneActor()
{
	const FActorSpawnParameters SpawnInfo;
	const FRotator MyRot(0, 0, 0);
	const FVector MyLoc(0, 0, 0);

	AARPlaneActor* CustomPlane = GetWorld()->SpawnActor<AARPlaneActor>(MyLoc, MyRot, SpawnInfo);

	return CustomPlane;
}

//Gets the colour to set the plane to when its spawned
FColor AHelloARManager::GetPlaneColor(int Index)
{
	return PlaneColors[Index % PlaneColors.Num()];
}

void AHelloARManager::ResetARCoreSession()
{

	//Get all actors in the level and destroy them as well as emptying the respective arrays
	TArray<AActor*> Planes;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AARPlaneActor::StaticClass(), Planes);

	for ( auto& It : Planes)
		It->Destroy();
	
	Planes.Empty();
	PlaneActors.Empty();

}
