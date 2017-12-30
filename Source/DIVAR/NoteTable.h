#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Engine/DataTable.h"
#include "WidgetNoteBase.h"
#include "NoteTable.generated.h"

/** Structure to store the lookup of GameObjects for use in a UDataTable */
USTRUCT(Blueprintable)
struct FNoteTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(BlueprintReadOnly)
	float timestamp;

	UPROPERTY(BlueprintReadOnly)
	EType type;

	UPROPERTY(BlueprintReadOnly)
	float holdLength;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHoldEnd;

	UPROPERTY(BlueprintReadOnly)
	float posX;

	UPROPERTY(BlueprintReadOnly)
	float posY;

	UPROPERTY(BlueprintReadOnly)
	float entryAngle;

	UPROPERTY(BlueprintReadOnly)
	float oscillationFrequency;

	UPROPERTY(BlueprintReadOnly)
	float oscillationAngle;

	UPROPERTY(BlueprintReadOnly)
	float oscillationAmplitude;

	UPROPERTY(BlueprintReadOnly)
	float timeout;
};