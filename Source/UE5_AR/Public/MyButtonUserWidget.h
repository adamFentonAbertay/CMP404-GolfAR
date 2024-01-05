// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MyButtonUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UMyButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButton* Button_0;


	UFUNCTION(BlueprintCallable, Category = "Score")
	virtual void buttonPressed();

	UFUNCTION()
	void OnButtonClick(); // Function to be called on button click
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	bool operationState = false;
};
