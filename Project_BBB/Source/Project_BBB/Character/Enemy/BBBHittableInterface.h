
#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BBBHittableInterface.generated.h"


UINTERFACE(MinimalAPI)
class UBBBHittableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PROJECT_BBB_API IBBBHittableInterface
{
	GENERATED_BODY()

public:
	virtual void OnHitByProjectile(AActor* Projectile) = 0;
};
