// Fill out your copyright notice in the Description page of Project Settings.


#include "MyButtonUserWidget.h"

void UMyButtonUserWidget::buttonPressed() {
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("button press"));

	if (operationState == true) {
		operationState = false;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball physics off"));
	}
	else {
		operationState = true;
		GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("ball physics on"));
	}
}








void UMyButtonUserWidget::OnButtonClick()
{
    // Handle button click here
	GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::White, TEXT("button press"));
}

