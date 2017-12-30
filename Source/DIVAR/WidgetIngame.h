// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetIngame.generated.h"

UENUM(BlueprintType)
enum class ESongDifficulty : uint8
{
	EASY,
	NORMAL,
	HARD,
	EXTREME
};

UENUM(BlueprintType)
enum class EClearResult : uint8
{
	MISSTAKE,
	CHEAP,
	STANDARD,
	GREAT,
	EXCELLENT,
	PERFECT
};

/**
 * 
 */
UCLASS()
class DIVAR_API UWidgetIngame : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText SongTitle;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	ESongDifficulty Difficulty;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	FText LyricText;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int Score;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	float SongProgress;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	EClearResult CurrentRank;
	
};
