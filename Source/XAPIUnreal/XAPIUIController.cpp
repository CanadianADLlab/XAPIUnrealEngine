// Fill out your copyright notice in the Description page of Project Settings.
 

#include "XAPIUIController.h"

// Sets default values for this component's properties
UXAPIUIController::UXAPIUIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UXAPIUIController::BeginPlay()
{
	Super::BeginPlay();

	SetUpUI();
}


// Called every frame
void UXAPIUIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UXAPIUIController::RequestBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Sending XAPI Phrase"));
	FString CurrentDateTime =  FDateTime::Now().ToString();
	if (XAPIMananger)
	{
		XAPIMananger->CreateXAPIPhrase("unreallevel", "test agent", "notreal@email.com", "completed","http://adlnet.gov/expapi/verbs/","http://adlnet.gov/expapi/activities/course", GetWorld()->GetTimeSeconds(), CurrentDateTime);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ERROR XAPI CONTROLLER IS NULL ON ACTOR %s please set in the inspector"),*GetOwner()->GetName());
	}
}

void UXAPIUIController::SetUpUI()
{
	XAPIUIClass = LoadClass<UUserWidget>(nullptr, TEXT("XAPIUIWidget'/Game/Game/UI/XAPIUIWidget.XAPIUIWidget_C'"));

	if (XAPIUIClass)
	{
		XAPIUIWidget = CreateWidget(GetWorld()->GetFirstPlayerController(), XAPIUIClass);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find UI WIdget on actor %s"), *GetOwner()->GetName());
	}
	if (XAPIUIWidget)
	{
		FScriptDelegate sgbDelegate;
		UButton* RequestBtn = (UButton*)XAPIUIWidget->GetWidgetFromName(TEXT("RequestBtn"));
		if (RequestBtn)
		{
			sgbDelegate.BindUFunction(this, "RequestBtnClicked");
			RequestBtn->OnClicked.Add(sgbDelegate);
		}
		XAPIUIWidget->AddToViewport();
		ShowMouse();
	}
}


void UXAPIUIController::ShowMouse()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

