// Fill out your copyright notice in the Description page of Project Settings.


#include "XAPIController.h"
#include "Misc/DateTime.h"

// Sets default values for this component's properties
UXAPIController::UXAPIController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UXAPIController::CreateXAPIPhrase(FString Activity,FString AgentName,FString Email,FString VerbName,float TimeToComplete,FDateTime CurrentDateTime)
{
	if (AgentName.IsEmpty())
	{
		AgentName = TEXT("NOAGENT");
	}

	if (Activity.IsEmpty())
	{
		Activity = TEXT("NOACTIVITY");
	}

	if (TimeToComplete == 0)
	{
		TimeToComplete = GetWorld()->GetTimeSeconds();
	}

	if (Email.IsEmpty())
	{
		Email = TEXT("NoEmail@email.com");
	}

	FString FormattedDateTime = "";
	if (CurrentDateTime != NULL)
	{
		FormattedDateTime = CurrentDateTime.ToIso8601();
	}

	FString LevelDuration = FString::SanitizeFloat(TimeToComplete);

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
		TEXT("\"result\": { ")
		TEXT("\"duration\": \"PT" + LevelDuration + "S\",");
		

	if (VerbName.ToLower().Equals("completed"))
	{
		XAPIJson += TEXT("\"completion\": true, ")
			TEXT("\"success\" : true ");
	} 
	if (!FormattedDateTime.IsEmpty()) // if someone sets the datetime than we override the current date time string
	{
		XAPIJson += TEXT("},\"timestamp\": \"" + FormattedDateTime + "\"");
	}
	else
	{
		XAPIJson += TEXT("}");
	}
	XAPIJson += TEXT("}");

	UE_LOG(LogTemp, Warning, TEXT("The json string is %s"), *XAPIJson);
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

	FDateTime CurrentDateTime = FDateTime::Now();
	UE_LOG(LogTemp, Error, TEXT("Current date time  is  %s"), *CurrentDateTime.ToString());
	CreateXAPIPhrase("unreallevel", "shuuun", "notreal@email.com", "completed",GetWorld()->GetTimeSeconds(), CurrentDateTime);
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

