// BBBDebuffTypes.h

#pragma once

#include "CoreMinimal.h"
#include "BBBDebuffTypes.generated.h"

UENUM(BlueprintType)
enum class EDebuffType : uint8
{
    None        UMETA(DisplayName = "None"),
    Stun        UMETA(DisplayName = "Stun"),           // 기절 (이동/공격 불가)
    Slow        UMETA(DisplayName = "Slow"),           // 이동속도 감소
    Weaken      UMETA(DisplayName = "Weaken"),         // 방어력 감소
    Blind       UMETA(DisplayName = "Blind"),          // 시야 감소 (AI용)
    DOT         UMETA(DisplayName = "DOT")             // 지속 데미지 (선택)
};

USTRUCT(BlueprintType)
struct FDebuffData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EDebuffType DebuffType;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Duration;          // 지속 시간

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    float Magnitude;         // 효과 크기 (50% = 0.5)

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool bStackable;         // 중첩 가능 여부

    FDebuffData()
        : DebuffType(EDebuffType::None)
        , Duration(3.0f)
        , Magnitude(0.5f)
        , bStackable(false)
    {
    }
};