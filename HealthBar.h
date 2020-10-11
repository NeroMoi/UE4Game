// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include"Runtime/UMG/Public/Components/ProgressBar.h"
#include "HealthBar.generated.h"

/**
 * 
 */
UCLASS()
class AITEST_API UHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
public:
	
	void NativeConstruct() override;

	void SetBarValuePercent(float const value);

protected:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
		UProgressBar * healthValue = nullptr;

};

