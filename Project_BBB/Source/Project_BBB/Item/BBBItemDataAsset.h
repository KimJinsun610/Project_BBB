// BBBItemDataAsset.h
#pragma once

#include "CoreMinimal.h"
#include "BBBItemBase.h"
#include "BBBItemDataAsset.generated.h" // ← 파일명과 반드시 일치해야 함


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