// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/BBBItemTypes.h"

#include "BBBInventoryComponent.generated.h"


// 인벤토리 변경 시 UI 갱신용
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedSignature);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT_BBB_API UBBBInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UBBBInventoryComponent();

protected:
	virtual void BeginPlay() override;

//================================================
// Data Section
public:
	// 에디터에서 아이템 DataTable 연결
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	TObjectPtr<class UDataTable> ItemDataTable;

	// 인벤토리 슬롯 목록
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	TArray<FBBBInventorySlot> Slots;
		
//================================================
// Event Section
public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChangedSignature OnInventoryChanged;


//================================================
// Inventory Section
public:
	// 아이템 추가 (스택 처리 포함)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool AddItem(FName ItemID, int32 Count = 1);

	// 아이템 제거
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(FName ItemID, int32 Count = 1);

	// 보유 여부 확인
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool HasItem(FName ItemID, int32 Count = 1) const;

	// 아이템 사용 (효과 적용 + 수량 차감)
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItem(FName ItemID);

//================================================
// Utility Section
private:
	// DataTable에서 아이템 데이터 조회
	FBBBItemData* FindItemData(FName ItemID) const;

	// 슬롯 인덱스 반환 (-1이면 없음)
	int32 FindSlotIndex(FName ItemID) const;

	// 아이템 효과 실제 적용
	void ApplyItemEffect(const FBBBItemData& ItemData);
};
