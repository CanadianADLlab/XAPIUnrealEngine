// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "HttpModule.h"
#include "JsonObjectConverter.h"
#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"
#include "XAPIManager.generated.h"

UCLASS()
class XAPIUNREAL_API AXAPIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AXAPIManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	FHttpModule* Http;
	FHttpRequestRef RequestWithRoute(FString Subroute);
	FHttpRequestRef GetRequest(FString Subroute);
	FHttpRequestRef PostRequest(FString Subroute, FString ContentJsonString);
	void Send(FHttpRequestRef& Request);
	bool ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CreateXAPIPhrase(FString Activity = "", FString AgentName = "", FString Email = "", FString VerbName = "", float TimeToComplete = 0, FDateTime CurrentDateTime = NULL);
private:

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString URL = "";

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString BasicAuth = "";

	UPROPERTY(EditAnywhere, Category = XAPISettings)
	FString XAPIVersion = "1.0.3";

	void XAPILRSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

};
