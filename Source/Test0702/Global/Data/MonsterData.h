#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "../GlobalEnums.h"
#include "MonsterData.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TEST0702_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterData() {}
	~FMonsterData() {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int HP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int ATT;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBehaviorTree* AI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<AIState, class UAnimMontage*> MapAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float SearchRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float AttackRange = 200.0f;
};

