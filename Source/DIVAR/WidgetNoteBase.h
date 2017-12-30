// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Animation/WidgetAnimation.h"
#include "WidgetNoteBase.generated.h"

UENUM(BlueprintType)
enum class EType : uint8
{
	TRIANGLE = 0,
	CIRCLE = 1,
	CROSS = 2,
	SQUARE = 3,
	TRIANGLE_DOUBLE = 4,
	CIRCLE_DOUBLE = 5,
	CROSS_DOUBLE = 6,
	SQUARE_DOUBLE = 7,
	TRIANGLE_HOLD = 8,
	CIRCLE_HOLD = 9,
	CROSS_HOLD = 10,
	SQUARE_HOLD = 11,
	STAR = 12,
	STAR_DOUBLE = 14,
	CHANCE_STAR = 15,
	LINKED_STAR = 22,
	LINKED_STAR_END = 23
};

/**
 * 
 */
UCLASS()
class DIVAR_API UWidgetNoteBase : public UUserWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(BlueprintReadOnly)
	EType type;

	UPROPERTY(BlueprintReadOnly)
	float flyProgress = 1.0;

	UPROPERTY(BlueprintReadOnly)
	float entryAngle;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHold;

	UPROPERTY(BlueprintReadOnly)
	bool bIsHoldEnd;

	UPROPERTY(BlueprintReadOnly)
	float holdLength = 0;

	UPROPERTY(BlueprintReadOnly)
	float timeOut = 1.5;

	UPROPERTY(BlueprintReadOnly)
	float oscillationFade = 1;

	UPROPERTY(BlueprintReadOnly)
	float oscillationAngle = 1.5;

	UPROPERTY(BlueprintReadOnly)
	float oscillationFrequency = 2;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* NoteBaseMaterial;

	UPROPERTY(EditDefaultsOnly)
	UMaterialInterface* TrailBaseMaterial;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* NoteMaterial;

	UPROPERTY(BlueprintReadOnly)
	UMaterialInstanceDynamic* TrailMaterial;

	UPROPERTY(BlueprintReadWrite)
	UWidgetAnimation* fadeIn;

	UPROPERTY(BlueprintReadOnly)
	FVector2D position;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bCanDestroy = true;

private:
	UWidget* TrailWidget;

public:
	UFUNCTION(BlueprintCallable)
	void NoteConstruct(UWidget* trailWidget);

	UFUNCTION(BlueprintCallable)
	void NoteTick(float deltaTime, UWidget* timerWidget);

	UFUNCTION(BlueprintCallable)
	void SetPosition(FVector2D pos);
};
