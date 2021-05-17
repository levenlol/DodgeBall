// Fill out your copyright notice in the Description page of Project Settings.


#include "Observation.h"

UObservation::UObservation()
{
}

void UObservation::Init()
{

}

TArray<float> UObservation::ComputeObservation() const
{
	check(0 && "Need to override ComputeObservation.");
	return {};
}
