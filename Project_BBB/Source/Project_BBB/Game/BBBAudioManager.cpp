#include "Game/BBBAudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"


// Sets default values for this component's properties
UBBBAudioManager::UBBBAudioManager()
{
	PrimaryComponentTick.bCanEverTick = false;

}

void UBBBAudioManager::PlayBGM(USoundBase* BGM)
{
    if (!BGM) return;

    if (BGMAudioComponent)
    {
        BGMAudioComponent->Stop();
    }

    BGMAudioComponent = UGameplayStatics::SpawnSound2D(this, BGM, 0.7f, 1.0f, 0.0f, nullptr, true, true);
}

void UBBBAudioManager::StopBGM()
{
    if (BGMAudioComponent)
    {
        BGMAudioComponent->Stop();
    }
}

void UBBBAudioManager::PlayUIClickSFX()
{
    if (!UIClickSFX) return;
    UGameplayStatics::PlaySound2D(this, UIClickSFX);
}
