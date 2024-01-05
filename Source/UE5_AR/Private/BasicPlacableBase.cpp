// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlacableBase.h"
#include "ARPin.h"
#include "ARBlueprintLibrary.h"

// Sets default values
ABasicPlacableBase::ABasicPlacableBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	/*PrimaryActorTick.bCanEverTick = true;



	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SetRootComponent(SceneComponent);*/


}

// Called when the game starts or when spawned
void ABasicPlacableBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABasicPlacableBase::Tick(float DeltaTime)
{
	//Super::Tick(DeltaTime);


	//// Making sure the actor remains on the ARPin that has been found.
	//if (PinComponent)
	//{
	//	auto TrackingState = PinComponent->GetTrackingState();

	//	switch (TrackingState)
	//	{
	//	case EARTrackingState::Tracking:
	//		SceneComponent->SetVisibility(true);
	//		SetActorTransform(PinComponent->GetLocalToWorldTransform());

	//		// Scale down default cube mesh - Change this for your applications.
	//		SetActorScale3D(FVector(0.2f, 0.2f, 0.2f));
	//		break;

	//	case EARTrackingState::NotTracking:
	//		PinComponent = nullptr;

	//		break;

	//	}
	//}
}

