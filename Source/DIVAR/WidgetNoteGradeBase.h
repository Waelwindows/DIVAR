// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WidgetNoteGradeBase.generated.h"

UENUM(BlueprintType)
enum class EGrade : uint8
{
	COOL,
	FINE,
	SAFE,
	BAD,
	MISS
};

/**
*
*/
UCLASS()
class DIVAR_API UWidgetNoteGradeBase : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	EGrade grade;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int combo;
};
