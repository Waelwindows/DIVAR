// Fill out your copyright notice in the Description page of Project Settings.

#include "WidgetNoteBase.h"

void UWidgetNoteBase::NoteConstruct(UWidget* trailWidget)
{
	if (NoteBaseMaterial)
		NoteMaterial = UMaterialInstanceDynamic::Create(NoteBaseMaterial, this);
	if (TrailBaseMaterial)
		TrailMaterial = UMaterialInstanceDynamic::Create(TrailBaseMaterial, this);
	if (!NoteMaterial) { return; }
	if (!TrailMaterial) { return; }

	NoteMaterial->SetScalarParameterValue("Entry", entryAngle);	
	NoteMaterial->SetScalarParameterValue("Fly", flyProgress);
	NoteMaterial->SetScalarParameterValue("Frequency", oscillationFrequency);

	trailWidget->SetRenderAngle((entryAngle * 57.3) - 90);
	TrailWidget = trailWidget;

	if (fadeIn)
		this->PlayAnimation(fadeIn, 0, 1, EUMGSequencePlayMode::Type::Forward, 1);
}

void UWidgetNoteBase::SetPosition(FVector2D pos)
{
	//FVector2D viewPosition = pos * FVector2D(27, 23.3);
	FVector2D viewPosition = pos * FVector2D(40, 40);
	//NoteMaterial->SetVectorParameterValue("FlyStart", FLinearColor(viewPosition.X + 100, viewPosition.Y + 100, 0, 0));
	this->SetPositionInViewport(viewPosition, false);
	position = viewPosition;
}

void UWidgetNoteBase::NoteTick(float deltaTime, UWidget* timerWidget)
{
	NoteMaterial->SetScalarParameterValue("Fly", flyProgress);
	NoteMaterial->SetScalarParameterValue("OscillStrength", oscillationFade);

	TrailMaterial->SetScalarParameterValue("Entry", entryAngle);
	TrailMaterial->SetScalarParameterValue("Fly", flyProgress);
	TrailMaterial->SetScalarParameterValue("Frequency", oscillationFrequency);
	TrailMaterial->SetScalarParameterValue("OscillStrength", oscillationFade);

	flyProgress -= deltaTime * (1 / timeOut);

	timerWidget->SetRenderAngle((1 - flyProgress) * 360);

	float oscillFadeStart = .1;
	float oscillFadeStep = .01;
	if (flyProgress <= oscillFadeStart && oscillationFade > oscillFadeStep)
		oscillationFade -= oscillFadeStep;

	bool bDoneFadeOut = false;
	if (flyProgress <= 0 && !bDoneFadeOut)
		if (fadeIn)
			this->PlayAnimation(fadeIn, 0, 1, EUMGSequencePlayMode::Type::Reverse, 1);
	bDoneFadeOut = true;
}



