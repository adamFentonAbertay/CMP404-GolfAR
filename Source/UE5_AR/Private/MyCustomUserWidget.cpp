// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCustomUserWidget.h"



void UMyCustomUserWidget::setProgVal(float val)
{
	
	ProgBar->SetPercent(val / 5);
}
