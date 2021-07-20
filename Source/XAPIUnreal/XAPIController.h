// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Blueprint/UserWidget.h"
#include "Components/ActorComponent.h"
#include "Components/Button.h"
#include "Engine/World.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "XAPIController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XAPIUNREAL_API UXAPIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXAPIController();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	FHttpModule* Http;
	FHttpRequestRef RequestWithRoute(FString Subroute);
	FHttpRequestRef GetRequest(FString Subroute);
	FHttpRequestRef PostRequest(FString Subroute, FString ContentJsonString);
	void Send(FHttpRequestRef& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private: 

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString URL = "https://lrs.ongarde.net/data/xAPI/statements";
	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString StoreKey = "b17e88c38cdb2ddab559c97ce8ea95e77e7a622c4";
	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString StoreSecretKey = "844bc137fa9d61a43618bff31577e09b29ccbb84";

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString BasicAuth = "Basic YjE3ZTg4YzM4Y2RiMmRkYWI1NTljOTdjZThlYTk1ZTc3ZTdhNjIyYzo4NDRiYzEzN2ZhOWQ2MWE0MzYxOGJmZjMxNTc3ZTA5YjI5Y2NiYjg0";

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString XAPIVersion = "1.0.3";

	UFUNCTION()
	void RequestBtnClicked();

	void XAPILRSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

	void SetUpUI();
	void ShowMouse();

	FString XAPIPHRASE = "";

	TSubclassOf<UUserWidget> XAPIUIClass = nullptr;
	UUserWidget* XAPIUIWidget;


	void CreateXAPIPhrase(FString Activity = "", FString AgentName = "", FString Email = "", FString VerbName = "", float TimeToComplete = 0, FDateTime CurrentDateTime = NULL);

};
