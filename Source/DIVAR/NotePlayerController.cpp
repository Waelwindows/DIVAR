// Fill out your copyright notice in the Description page of Project Settings.

#include "NotePlayerController.h"

ANotePlayerController::ANotePlayerController()
{
	noteAudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Note Audio Component"));
	noteAudioComponent->bAutoActivate = false;

	static ConstructorHelpers::FObjectFinder<UDataTable>GameObjectLookupDataTable_BP(TEXT("DataTable'/Game/DIVAR/Script/pv_724_extreme.pv_724_extreme'"));
	noteSource = GameObjectLookupDataTable_BP.Object;

	//static ConstructorHelpers::FObjectFinder<TSubclassOf<class UUserWidget>>LookUpCircleWidget(TEXT("UserWidget'/Game/UserInterface/Note/UIW_Note_Circle.UIW_Note_Circle'"));
	//CircleWidget = *LookUpCircleWidget.Object;
}

void ANotePlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (noteAudioCue->IsValidLowLevelFast())
		noteAudioComponent->SetSound(noteAudioCue);
}

void ANotePlayerController::BeginPlay()
{
	Super::BeginPlay();

	static const FString ContextString(TEXT("GENERAL"));

	TArray<FNoteTable*> noteTables;

	IngameUI = CreateWidget<UWidgetIngame>(this, IngameUIWidget);
	IngameUI->AddToViewport();

	IngameUI->SongTitle = NSLOCTEXT("Your Namespace", "123", "Test song");
	IngameUI->LyricText = NSLOCTEXT("Your Namespace", "345", "This is a normal song lyric");

	for (auto rowName : noteSource->GetRowNames())
		noteTables.Add(noteSource->FindRow<FNoteTable>(rowName, ContextString));

	for (auto currentNote : noteTables)
	{
		CreateNoteTime(*currentNote);
	}
}

void ANotePlayerController::Tick(float deltaTime)
{
	Super::Tick(deltaTime);

	GEngine->AddOnScreenDebugMessage(-1, 0, FColor::Yellow, "Tests");

	if (Notes.Num() > 0)
	{
		if (Notes[0]->flyProgress < -0.1 && !Notes[0]->bIsHold)
		{
			//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Remove");
			CreateGrade(EGrade::MISS);
			Notes[0]->RemoveFromParent();
			Notes.RemoveAt(0);
			Combo = 0;
		}
	}
}

///////////////////////////////////////////////////////////////////////

void ANotePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UInputComponent* Input = this->InputComponent;

	Input->BindAction("Circle", IE_Pressed, this, &ANotePlayerController::HitNote<EType::CIRCLE, false, true>);
	Input->BindAction("Circle", IE_Released, this, &ANotePlayerController::HitNote<EType::CIRCLE, false, false>);

	Input->BindAction("PAD-Right", IE_Pressed, this, &ANotePlayerController::HitNote<EType::CIRCLE, true, true>);
	Input->BindAction("PAD-Right", IE_Released, this, &ANotePlayerController::HitNote<EType::CIRCLE, true, false>);

	Input->BindAction("Cross", IE_Pressed, this, &ANotePlayerController::HitNote<EType::CROSS, false, true>);
	Input->BindAction("Cross", IE_Released, this, &ANotePlayerController::HitNote<EType::CROSS, false, false>);

	Input->BindAction("PAD-Down", IE_Pressed, this, &ANotePlayerController::HitNote<EType::CROSS, true, true>);
	Input->BindAction("PAD-Down", IE_Released, this, &ANotePlayerController::HitNote<EType::CROSS, true, false>);

	Input->BindAction("Square", IE_Pressed, this, &ANotePlayerController::HitNote<EType::SQUARE, false, true>);
	Input->BindAction("Square", IE_Released, this, &ANotePlayerController::HitNote<EType::SQUARE, false, false>);

	Input->BindAction("PAD-Left", IE_Pressed, this, &ANotePlayerController::HitNote<EType::SQUARE, true, true>);
	Input->BindAction("PAD-Left", IE_Released, this, &ANotePlayerController::HitNote<EType::SQUARE, true, false>);

	Input->BindAction("Triangle", IE_Pressed, this, &ANotePlayerController::HitNote<EType::TRIANGLE, false, true>);
	Input->BindAction("Triangle", IE_Released, this, &ANotePlayerController::HitNote<EType::TRIANGLE, false, false>);

	Input->BindAction("PAD-Up", IE_Pressed, this, &ANotePlayerController::HitNote<EType::TRIANGLE, true, true>);
	Input->BindAction("PAD-Up", IE_Released, this, &ANotePlayerController::HitNote<EType::TRIANGLE, true, false>);

	Input->BindAction("RIGHT_Flick", IE_Pressed, this, &ANotePlayerController::HitNote<EType::STAR, false, true>);
	Input->BindAction("RIGHT_Flick", IE_Released, this, &ANotePlayerController::HitNote<EType::STAR, false, false>);

	Input->BindAction("LEFT_FLICK", IE_Pressed, this, &ANotePlayerController::HitNote<EType::STAR, true, true>);
	Input->BindAction("LEFT_FLICK", IE_Released, this, &ANotePlayerController::HitNote<EType::STAR, true, false>);
}

///////////////////////////////////////////////////////////////////////

void ANotePlayerController::CreateNoteTime(FNoteTable noteData)
{
	FTimerHandle NoteCreateHandler;
	FTimerHandle NotePlayHandler;
	FTimerDelegate TimeDel;
	TimeDel.BindUFunction(this, FName("CreateNote"), noteData);

	GetWorldTimerManager().SetTimer(NoteCreateHandler, TimeDel, noteData.timestamp, false);
	GetWorldTimerManager().SetTimer(NotePlayHandler, this, &ANotePlayerController::HitNote, noteData.timestamp + noteData.timeout, false);
}


void ANotePlayerController::CreateNote(FNoteTable noteData)
{
	auto type = noteData.type;
	auto timeout = noteData.timeout;
	auto position = FVector2D(noteData.posX, noteData.posY);
	UWidgetNoteBase* note = CreateWidget<UWidgetNoteBase>(this, CircleWidget);
	switch (type)
	{
		case EType::CIRCLE: note = CreateWidget<UWidgetNoteBase>(this, CircleWidget); break;
		case EType::CIRCLE_DOUBLE: note = CreateWidget<UWidgetNoteBase>(this, CircleDoubleWidget); break;
		case EType::CIRCLE_HOLD: note = CreateWidget<UWidgetNoteBase>(this, CircleHoldWidget); break;
		case EType::CROSS: note = CreateWidget<UWidgetNoteBase>(this, CrossWidget); break;
		case EType::CROSS_DOUBLE: note = CreateWidget<UWidgetNoteBase>(this, CrossDoubleWidget); break;
		case EType::SQUARE: note = CreateWidget<UWidgetNoteBase>(this, SquareWidget); break;
		case EType::SQUARE_DOUBLE: note = CreateWidget<UWidgetNoteBase>(this, SquareDoubleWidget); break;
		case EType::TRIANGLE: note = CreateWidget<UWidgetNoteBase>(this, TriangleWidget); break;
		case EType::TRIANGLE_DOUBLE: note = CreateWidget<UWidgetNoteBase>(this, TriangleDoubleWidget); break;
		case EType::STAR: note = CreateWidget<UWidgetNoteBase>(this, StarWidget); break;
		case EType::LINKED_STAR: note = CreateWidget<UWidgetNoteBase>(this, StarWidget); break;
		case EType::LINKED_STAR_END: note = CreateWidget<UWidgetNoteBase>(this, StarWidget); break;
		case EType::STAR_DOUBLE: note = CreateWidget<UWidgetNoteBase>(this, StarDoubleWidget); break;
		default: note = CreateWidget<UWidgetNoteBase>(this, CircleWidget); break;
	}
	note->type = type;
	if (type == EType::CIRCLE_HOLD || type == EType::CROSS_HOLD || type == EType::SQUARE_HOLD || type == EType::TRIANGLE_HOLD)
		note->bIsHold = true;
	if (noteData.bIsHoldEnd)
		note->bIsHold = false;
	note->bIsHold = false;
	switch (type)
	{
		case EType::CIRCLE_HOLD: type = EType::CIRCLE; break;
		case EType::CROSS_HOLD: type = EType::CROSS; break;
		case EType::SQUARE_HOLD: type = EType::SQUARE; break;
		case EType::TRIANGLE_HOLD: type = EType::TRIANGLE; break;
	}
	if (type == EType::LINKED_STAR || type == EType::LINKED_STAR_END || type == EType::CHANCE_STAR)
		note->type = EType::STAR;
	if (type == EType::CIRCLE_HOLD || type == EType::CROSS_HOLD || type == EType::SQUARE_HOLD || type == EType::TRIANGLE_HOLD)
		note->type = EType::CIRCLE;
	
	note->entryAngle = noteData.entryAngle;
	note->timeOut = timeout;

	note->AddToViewport();
	note->SetPosition(position);
	Notes.Add(note);
}

void ANotePlayerController::HitNote()
{
	IConsoleManager::Get().RegisterConsoleVariable(TEXT("AutoPlay"), 0,
		TEXT("Playes the incoming notes as cool.\n")
		TEXT("0 = Normal play")
		TEXT("1 = Autoplay"),
		ECVF_Cheat);

	static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("AutoPlay"));
	auto bAutoPlayNotes = CVar->GetInt();

	if (Notes.Num() != 0 && bAutoPlayNotes == 1)
	{
		noteAudioComponent->Play();
		HitNoteBurst = CreateWidget<UUserWidget>(this, HitNoteWidget);
		HitNoteBurst->AddToViewport();
		HitNoteBurst->SetPositionInViewport(Notes[0]->position, false);
		++Combo;
		CreateGrade(EGrade::COOL);
		DestroyNote();
	}
}

void ANotePlayerController::OnHitNote(EType type)
{
	noteAudioComponent->Play();
	if (Notes.Num() != 0)
	{
		bool typeCondition = Notes[0]->type == type;
		switch (type)
		{
			case EType::CIRCLE_DOUBLE:   typeCondition = typeCondition || Notes[0]->type == EType::CIRCLE; break;
			case EType::CROSS_DOUBLE:    typeCondition = typeCondition || Notes[0]->type == EType::CROSS; break;
			case EType::SQUARE_DOUBLE:   typeCondition = typeCondition || Notes[0]->type == EType::SQUARE; break;
			case EType::TRIANGLE_DOUBLE: typeCondition = typeCondition || Notes[0]->type == EType::TRIANGLE; break;
			case EType::STAR_DOUBLE:     typeCondition = typeCondition || Notes[0]->type == EType::STAR; break;
		}

		bool bIsDouble = Notes[0]->type == EType::CIRCLE_DOUBLE || Notes[0]->type == EType::CROSS_DOUBLE || Notes[0]->type == EType::SQUARE_DOUBLE || Notes[0]->type == EType::TRIANGLE_DOUBLE;
		//DIVA COOL=0.03 FINE=0.07 SAFE=0.10 SAD=0.13

		float hitTime = Notes[0]->flyProgress;
		int coolScore = 200;
		int goodScore = 200;
		if (typeCondition && hitTime <= 0.13)
		{
			
			if (abs(hitTime) <= 0.03) { CreateGrade(EGrade::COOL); score += coolScore; }
			else if (abs(hitTime) <= 0.07) { CreateGrade(EGrade::FINE); }
			else if (abs(hitTime) <= 0.10) { CreateGrade(EGrade::SAFE); }
			else if (abs(hitTime) <= 0.13) { CreateGrade(EGrade::BAD); }
			else						   { CreateGrade(EGrade::MISS); }

			HitNoteBurst = CreateWidget<UUserWidget>(this, HitNoteWidget);
			HitNoteBurst->AddToViewport();
			HitNoteBurst->SetPositionInViewport(Notes[0]->position, false);
			++Combo;
			DestroyNote();
		}
		/*
		if (Notes[0]->flyProgress < 0.035 && typeCondition)
		{
			HitNote();
			return;
		}
		else if (Notes[0]->flyProgress < 0.035 && !bIsDouble)
		{
			CreateGrade(EGrade::MISS);
			DestroyNote();
		}
		*/
	}
}

void ANotePlayerController::DestroyNote(int NoteIndex)
{
	if (Notes.Num() > NoteIndex)
	{
		Notes[NoteIndex]->RemoveFromParent();
		Notes.RemoveAt(NoteIndex);
	}
}

void ANotePlayerController::CreateGrade(EGrade grade)
{
	if (NoteGrade) { NoteGrade->RemoveFromParent(); }
		NoteGrade = CreateWidget<UWidgetNoteGradeBase>(this, GradeWidget);
	if (grade == EGrade::MISS)
		Combo = 0;

	NoteGrade->combo = Combo;
	NoteGrade->grade = grade;
	NoteGrade->AddToViewport();
	NoteGrade->SetPositionInViewport(Notes[0]->position, false);
}