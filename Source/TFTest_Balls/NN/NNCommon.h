#pragma once

#include "CoreMinimal.h"
#include "NNCommon.generated.h"

USTRUCT()
struct TFTEST_BALLS_API FState
{
	GENERATED_BODY()
public:
	FState() = default;
	FState(const TArray<float>& InObservations, float InRewards, const TArray<float>& InActions)
	{
		Observations = InObservations;
		Rewards = InRewards;
		Actions = InActions;
	}

	const TArray<float>& GetObservations() const { return Observations; }
	float GetRewards() const { return Rewards; }
	const TArray<float>& GetActions() const { return Actions; }

private:
	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<float> Observations;

	UPROPERTY(VisibleAnywhere, Category = NN)
	float Rewards;

	UPROPERTY(VisibleAnywhere, Category = NN)
	TArray<float> Actions;
};

template<typename FmtType, typename ...Args>
void NN_Printf(const FmtType& s, Args&& ...args)
{
	UE_LOG(LogTemp, Log, s, args...);
}