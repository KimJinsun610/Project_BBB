// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Item/BBBItemTypes.h"

#include "BBBInventoryComponent.generated.h"


// 인벤토리 변경 시 UI 갱신용
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChangedSignature);

// 퀵슬롯
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuickSlotChangedSignature);

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
		

	// 퀵슬롯 3칸 고정 (index 0=좌, 1=중, 2=우)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "QuickSlot")
	TArray<FBBBQuickSlotData> QuickSlots;

	// 아이템 총 개수 확인용
	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<FBBBInventorySlot> GetMergedSlots() const;
//================================================
// Event Section
public:
	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChangedSignature OnInventoryChanged;


//================================================
// Inventory Section
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	int32 MaxSlotCount = 12;

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

	// 퀵슬롯
	UPROPERTY(BlueprintAssignable, Category = "QuickSlot")
	FOnQuickSlotChangedSignature OnQuickSlotChanged;

	//아이템 개수
	UFUNCTION(BlueprintPure, Category = "Inventory")
	int32 GetItemCount(FName ItemID) const;

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool UseItemAt(int32 SlotIndex);

//================================================
// Utility Section
public:
	// DataTable에서 아이템 데이터 조회
	FBBBItemData* FindItemData(FName ItemID) const;

	// 슬롯 인덱스 반환 (-1이면 없음)
	int32 FindSlotIndex(FName ItemID) const;

	// 아이템 효과 실제 적용
	void ApplyItemEffect(const FBBBItemData& ItemData);

	//BP에서 아이템 조회
	UFUNCTION(BlueprintPure, Category = "Inventory")
	bool GetItemData(FName ItemID, FBBBItemData& OutData) const;


	// 아이템 섭취 사운드
	UPROPERTY(EditDefaultsOnly, Category = "SFX")
	TObjectPtr<USoundBase> UseItemSFX;

//================================================
// QuickSlot Section
public:
	// 우측(2)부터 빈 슬롯 찾아 등록,  컨텍스트 메뉴 버튼용
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool RegisterQuickSlot(FName ItemID);

	// 지정 인덱스에 직접 등록, 드래그 앤 드롭용
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	void RegisterQuickSlotAt(FName ItemID, int32 SlotIndex);

	// 해당 슬롯 아이템 사용 (1/2/3 키 입력용)
	UFUNCTION(BlueprintCallable, Category = "QuickSlot")
	bool UseQuickSlot(int32 SlotIndex);
};
