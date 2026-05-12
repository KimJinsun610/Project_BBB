// BBBItemDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "BBBItemBase.h"
#include "BBBItemDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FItemDropData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<class ABBBItemBase> ItemClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float DropChance;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MinAmount;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 MaxAmount;

    FItemDropData()
        : DropChance(1.0f)
        , MinAmount(1)
        , MaxAmount(1)
    {
    }
};