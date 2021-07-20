// Fill out your copyright notice in the Description page of Project Settings.


#include "XAPIController.h"
#include "Misc/DateTime.h"

// Sets default values for this component's properties
UXAPIController::UXAPIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UXAPIController::CreateXAPIPhrase(FString Activity,FString AgentName,FString Email,FString VerbName,FString Type,float Time,FString NewDateTime)
{
	FString CurrentDateTime = FDateTime::Now().ToString();
	FString LevelDuration = FString::SanitizeFloat(GetWorld()->GetTimeSeconds());
	if (AgentName.IsEmpty())
	{
		AgentName = TEXT("NOAGENT");
	}

	if (Activity.IsEmpty())
	{
		Activity = TEXT("NOACTIVITY");
	}

	if (Time == 0)
	{
		Time = GetWorld()->GetTimeSeconds();
	}

	if (Email.IsEmpty())
	{
		Email = TEXT("NoEmail@email.com");
	}
	FString XAPIJson = TEXT("{")
		TEXT("\"actor\": {")
		TEXT("\"name\": \"" + AgentName + "\",")
		TEXT("\"mbox\": \"mailto:" + Email + "\"")
		TEXT("},")
		TEXT("\"verb\" : {")
		TEXT("\"id\": \"http://adlnet.gov/expapi/verbs/" + VerbName + "\",")
		TEXT("\"display\" : {\"en-US\": \"" + VerbName + "\"}")
		TEXT("},")
		TEXT("\"object\" : {")
		TEXT("\"id\": \"http://example.com/activities/solo-hang-gliding\",")
		TEXT("\"definition\" : { ")
		TEXT("\"type\": \"http://adlnet.gov/expapi/activities/course\",")
		TEXT("\"name\" : { \"en-US\":\" " + Activity + " \" }")
		TEXT("}")
		TEXT("},")
		TEXT("\"context\": {")
		TEXT("\"instructor\": {")
		TEXT("\"name\": \"Unrealed Instructor\",")
		TEXT("\"mbox\" : \"mailto:Unrealed@example.com\" ")
		TEXT("},")
		TEXT("\"contextActivities\" : { ")
		TEXT("\"parent\": { \"id\": \"http://example.com/activities/hang-gliding-class-a\" },")
		TEXT("\"grouping\" : { \"id\": \"http://example.com/activities/hang-gliding-school\" }")
		TEXT("}")
		TEXT("}")
		TEXT(",\"result\": { ")
		TEXT("\"duration\": \"PT" + LevelDuration + "S\",");
		

	if (VerbName.ToLower().Equals("completed"))
	{
		XAPIJson += TEXT("\"completion\": true, ")
			TEXT("\"success\" : true ");
	}
	XAPIJson += TEXT("}}");

	UE_LOG(LogTemp, Warning, TEXT("The json string is %s"), *XAPIJson);
	UE_LOG(LogTemp, Warning, TEXT("The current date time is %s"), *CurrentDateTime);
	if (Http)
	{
		UE_LOG(LogTemp, Warning, TEXT("Sending xapi stuff out %s"), *URL);
		FHttpRequestRef Request = PostRequest(TEXT(""), XAPIJson);

		Request->OnProcessRequestComplete().BindUObject(this, &UXAPIController::XAPILRSResponse); 
		Send(Request);
	}
}

void UXAPIController::XAPILRSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	UE_LOG(LogTemp, Warning, TEXT("Made it to the response %s"), *GetOwner()->GetName());

	if (!ResponseIsValid(Response, bWasSuccessful)) 
	{ return; 
	}
	UE_LOG(LogTemp, Warning, TEXT("Made it past the return %s"), *GetOwner()->GetName());
	FString JsonString = Response->GetContentAsString();  
	UE_LOG(LogTemp, Warning, TEXT("json response is %s"), *JsonString);
}

void UXAPIController::BeginPlay()
{
	Super::BeginPlay();
	Http = &FHttpModule::Get();
	if (!Http)
	{
		UE_LOG(LogTemp, Error, TEXT("http object is not valid for some reason on actor %s"), *GetOwner()->GetName());
	}
	SetUpUI();
	
}


// Called every frame
void UXAPIController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
}

void UXAPIController::RequestBtnClicked()
{
	UE_LOG(LogTemp, Warning, TEXT("Sending XAPI Phrase"));
	CreateXAPIPhrase("testscene", "shaun", "notreal@email.com", "completed");
	// ...
}

void UXAPIController::SetUpUI()
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


void UXAPIController::ShowMouse()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
}

FHttpRequestRef UXAPIController::RequestWithRoute(FString  Subroute)
{
	FHttpRequestRef Request = Http->CreateRequest();
	Request->SetURL(URL + Subroute);
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Authorization"),BasicAuth);
	Request->SetHeader(TEXT("X-EXPERIENCE-API-VERSION"), XAPIVersion);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	return Request;
}

FHttpRequestRef UXAPIController::GetRequest(FString Subroute)
{
	FHttpRequestRef Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

FHttpRequestRef UXAPIController::PostRequest(FString Subroute, FString ContentJsonString)
{
	FHttpRequestRef Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	//Request->SetContentAsString();

	return Request;
}

void UXAPIController::Send(FHttpRequestRef& Request)
{
	Request->ProcessRequest();
}

bool UXAPIController::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
{
	if ((!bWasSuccessful) || (!Response.IsValid()))
	{
		return false;
	}
	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Http message code:  %s"), *Response->GetContentAsString());
		UE_LOG(LogTemp, Warning, TEXT("Http Response returned error code:  %d"), Response->GetResponseCode());  
		return false;
	}
}

