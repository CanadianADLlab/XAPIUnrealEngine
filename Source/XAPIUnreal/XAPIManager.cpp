// Fill out your copyright notice in the Description page of Project Settings.


#include "XAPIManager.h"

// Sets default values
AXAPIManager::AXAPIManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AXAPIManager::BeginPlay()
{
	Super::BeginPlay();

	if (URL.IsEmpty() || BasicAuth.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Either the lrs url or the basic auth is not set on the object %s please set or xapi statements WONT WORK"), *GetName());
	}
	
}
// Called every frame
void AXAPIManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AXAPIManager::CreateXAPIPhrase(FString Activity, FString AgentName, FString Email, FString VerbName, FString VerbURL,FString CourseURL ,float TimeToComplete, FString CurrentDateTime)
{

	if (TimeToComplete == 0)
	{ 
		TimeToComplete = GetWorld()->GetTimeSeconds();
	}

	FString FormattedDateTime = "";
	if (!CurrentDateTime.IsEmpty())
	{
		FDateTime currentDate;
		FDateTime::Parse(CurrentDateTime, currentDate);
		FormattedDateTime = currentDate.UtcNow().ToIso8601();
	}

	FString LevelDuration = FString::SanitizeFloat(TimeToComplete);

	FString XAPIJson = TEXT("{")
		TEXT("\"actor\": {")
		TEXT("\"name\": \"" + AgentName + "\",")
		TEXT("\"mbox\": \"mailto:" + Email + "\"")
		TEXT("},")
		TEXT("\"verb\" : {")
		TEXT("\"id\": \"" + VerbURL + VerbName + "\",")
		TEXT("\"display\" : {\"en-US\": \"" + VerbName + "\"}")
		TEXT("},")
		TEXT("\"object\" : {")
		TEXT("\"id\": \"http:// " + Activity + " \",") // not sure why the id has to be in the format of an url
		TEXT("\"definition\" : { ")
		TEXT("\"type\": \"" + CourseURL + " \",")
		TEXT("\"name\" : { \"en-US\":\" " + Activity + " \" }")
		TEXT("}")
		TEXT("},")
		TEXT("\"result\": { ")
		TEXT("\"duration\": \"PT" + LevelDuration + "S\"");


	if (VerbName.ToLower().Equals("completed"))
	{
		XAPIJson += TEXT(",\"completion\": true, ")
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

	Http = &FHttpModule::Get();
	if (Http)
	{
		FHttpRequestRef Request = PostRequest(TEXT(""), XAPIJson);
		Request->OnProcessRequestComplete().BindUObject(this, &AXAPIManager::XAPILRSResponse);
		Send(Request);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Http pointer null attatched to %s"), *GetName());
	}
}

void AXAPIManager::XAPILRSResponse(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (!ResponseIsValid(Response, bWasSuccessful))
	{
		return;
	}
	FString JsonString = Response->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("json response is %s"), *JsonString);
}




FHttpRequestRef AXAPIManager::RequestWithRoute(FString  Subroute)
{
	FHttpRequestRef Request = Http->CreateRequest();
	Request->SetURL(URL + Subroute);
	//Request->SetHeader(TEXT("User-Agent"), TEXT("X-UnrealEngine-Agent"));
	Request->SetHeader(TEXT("Authorization"), BasicAuth);
	Request->SetHeader(TEXT("X-EXPERIENCE-API-VERSION"), XAPIVersion);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
	return Request;
}

FHttpRequestRef AXAPIManager::GetRequest(FString Subroute)
{
	FHttpRequestRef Request = RequestWithRoute(Subroute);
	Request->SetVerb("GET");
	return Request;
}

FHttpRequestRef AXAPIManager::PostRequest(FString Subroute, FString ContentJsonString)
{
	FHttpRequestRef Request = RequestWithRoute(Subroute);
	Request->SetVerb("POST");
	Request->SetContentAsString(ContentJsonString);
	return Request;
}

void AXAPIManager::Send(FHttpRequestRef& Request)
{
	Request->ProcessRequest();
}

bool AXAPIManager::ResponseIsValid(FHttpResponsePtr Response, bool bWasSuccessful)
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



