// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/ActorComponent.h"
#include "XAPIManager.h"
#include "XAPIUIController.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class XAPIUNREAL_API UXAPIUIController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UXAPIUIController();
	 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(EditAnywhere)
	AXAPIManager* XAPIMananger;

private: 
	UFUNCTION()
	void RequestBtnClicked();

	
	TSubclassOf<UUserWidget> XAPIUIClass = nullptr;
	UUserWidget* XAPIUIWidget;

	void SetUpUI();
	void ShowMouse();

};
