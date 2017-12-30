// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/InputSettings.h"
#include "Runtime/UMG/Public/UMG.h"
#include "WidgetNoteBase.h"
#include "WidgetIngame.h"
#include "WidgetNoteGradeBase.h"
#include "NoteTable.h"
#include "NotePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class DIVAR_API ANotePlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Audio")
	UAudioComponent* noteAudioComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USoundWave* noteAudioCue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> IngameUIWidget;

	//////////////////////////////////////////////////////////////////

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UDataTable* noteSource;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> HitNoteWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> GradeWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CircleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CircleDoubleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CircleHoldWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CrossWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CrossDoubleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> CrossHoldWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> SquareWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> SquareHoldWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> SquareDoubleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> TriangleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> TriangleDoubleWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> TriangleHoldWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> StarWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<class UUserWidget> StarDoubleWidget;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly)
	UWidgetIngame* IngameUI;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly)
	TArray<UWidgetNoteBase*> Notes;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly)
	UWidgetNoteGradeBase* NoteGrade;

	UPROPERTY(VisibleInstanceOnly, BluePrintReadOnly)
	UUserWidget* HitNoteBurst;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	int Combo;

	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	int score;

protected:
	// APawn interface
	virtual void SetupInputComponent() override;
	// End of APawn interface

private:
	bool bIsHoldNote;
	float holdLength;
	float holdProgress;

	bool bHoldCircle;
	bool bHoldRight;

	bool bHoldCross;
	bool bHoldDown;

	bool bHoldSquare;
	bool bHoldLeft;

	bool bHoldTriangle;
	bool bHoldUp;

	bool bHoldStar;
public:
	ANotePlayerController();

	virtual void PostInitializeComponents() override;

	virtual void BeginPlay();
	virtual void Tick(float deltaTime);

	/*Creates a note and displays it*/
	UFUNCTION(BlueprintCallable)
	void CreateNoteTime(FNoteTable noteData);

	UFUNCTION(BlueprintCallable)
	void CreateNote(FNoteTable noteData);

	/*Hits any note type and gives Cool rank. Used by Autoplayer*/
	UFUNCTION(BlueprintCallable)
	void HitNote();

	void OnHitNote(EType type = EType::CIRCLE);

	void DestroyNote(int NoteIndex = 0);

	void CreateGrade(EGrade grade = EGrade::COOL);

	/**
	*Hits a note using the note type.
	*@param type The note type to hite
	*@param bSetPad - whether the hit note acts like a Double note or not
	*@param bState - whether to set the hold variable to true or not
	*/
	template<EType type, bool bSetPad, bool bState>
	void HitNote()
	{
		if (bSetPad)
		{
			switch (type)
			{
			case EType::CIRCLE: bHoldRight = bState; break;
			case EType::CROSS: bHoldDown = bState; break;
			case EType::SQUARE: bHoldLeft = bState; break;
			case EType::TRIANGLE: bHoldUp = bState; break;
			}
		}
		else
		{
			switch (type)
			{
			case EType::CIRCLE: bHoldCircle = bState; break;
			case EType::CROSS: bHoldCross = bState; break;
			case EType::SQUARE: bHoldSquare = bState; break;
			case EType::TRIANGLE: bHoldTriangle = bState; break;
			}
		}
		bool condition = false;
		switch (type)
		{
		case EType::CIRCLE: condition = bHoldCircle & bHoldRight; break;
		case EType::CROSS: condition = bHoldCross & bHoldDown; break;
		case EType::SQUARE: condition = bHoldSquare & bHoldLeft; break;
		case EType::TRIANGLE: condition = bHoldTriangle & bHoldUp; break;
		case EType::STAR: condition = bHoldStar; break;
		}
		if (bState)
		{
			if (condition)
				switch (type)
				{
				case EType::CIRCLE: OnHitNote(EType::CIRCLE_DOUBLE); break;
				case EType::CROSS: OnHitNote(EType::CROSS_DOUBLE); break;
				case EType::SQUARE: OnHitNote(EType::SQUARE_DOUBLE); break;
				case EType::TRIANGLE: OnHitNote(EType::TRIANGLE_DOUBLE); break;
				case EType::STAR: OnHitNote(EType::STAR_DOUBLE); break;
				}
			else
				OnHitNote(type);
		}
		else
		{
			if (Notes.Num() > 0)
			{
				if (Notes[0]->bIsHold)
				{
					OnHitNote(type);
				}
			}
		}
		if (type == EType::STAR)
		{
			bHoldStar = bState;
		}
	}
};
