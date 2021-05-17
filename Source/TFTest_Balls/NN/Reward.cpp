// Fill out your copyright notice in the Description page of Project Settings.


#include "Reward.h"

UReward::UReward()
{
}

void UReward::Init()
{

}

float UReward::ComputeReward() const
{
	check(0 && "Reward override ComputeReward()");
	return 0.f;
}
