// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaceableActor.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"

// Sets default values
APlaceableActor::APlaceableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);

	isShot = false;
	//StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//StaticMeshComponent->SetupAttachment(SceneComponent);

	//auto MeshAsset = ConstructorHelpers::FObjectFinder<UStaticMesh>(TEXT("StaticMesh'/Engine/BasicShapes/Sphere.Sphere'"));
	//StaticMeshComponent->SetStaticMesh(MeshAsset.Object);
	

}

// Called when the game starts or when spawned
void APlaceableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlaceableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// Making sure the actor remains on the ARPin that has been found.
	if(PinComponent)
	{
		auto TrackingState = PinComponent->GetTrackingState();
		
		/*if (isShot == true) {
			TrackingState = EARTrackingState::NotTracking;
		}*/
		switch (TrackingState)
		{
		case EARTrackingState::Tracking:
		
			SceneComponent->SetVisibility(true);
			//this->GetFName()
			if (isShot == true) {
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("ive been shot"));
			}
			else {


				//this is like always true
				// 
				// CUZ OF ARROW AS WELL
				//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: not shot so setting transform"));

				







				// UNCOMMENT
				//SetActorTransform(PinComponent->GetLocalToWorldTransform());






				TrackingState = EARTrackingState::NotTracking;
			}
			//else {
			//	//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball: shot"));
			//	PinComponent = nullptr;
			//}
			// Scale down default cube mesh - Change this for your applications.
			// 
			// 
			// 
			//this is why ball big
			//SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
			break;

		case EARTrackingState::NotTracking:
			PinComponent = nullptr;

			break;

		}
	}
	

}

