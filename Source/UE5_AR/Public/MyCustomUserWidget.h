// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyCustomUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class UE5_AR_API UMyCustomUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ProgBar;


	UFUNCTION(BlueprintCallable, Category = "Score")
	virtual void setProgVal(float val);
};
