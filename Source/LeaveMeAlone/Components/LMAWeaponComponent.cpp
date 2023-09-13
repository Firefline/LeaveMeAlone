// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "LMAWeaponComponent.h"
#include "GameFramework/Character.h"
#include "LeaveMeAlone/Weapon/LMABaseWeapon.h"
#include "LeaveMeAlone/Animations/LMAReloadFinishedAnimNotify.h"

// Sets default values for this component's properties
ULMAWeaponComponent::ULMAWeaponComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void ULMAWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SpawnWeapon();
    InitAnimNotify();
	
}


// Called every frame
void ULMAWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULMAWeaponComponent::SpawnWeapon() 
{
   Weapon = GetWorld()->SpawnActor<ALMABaseWeapon>(WeaponClass);
   Weapon->OnCurrentClipEmpty.AddUObject(this, &ULMAWeaponComponent::OnCurrentClipEmpty);

   if (Weapon)
   {
		const auto Character = Cast<ACharacter>(GetOwner());
		if (Character) 
		{
			FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
			Weapon->AttachToComponent(Character->GetMesh(), AttachmentRules, "r_Weapon_Socket");

		}

   }

}

void ULMAWeaponComponent::Fire() 
{
   if (Weapon && !AnimReloading) 
   {
		Weapon->Fire();

   }

}

void ULMAWeaponComponent::StopFire() 
{
	Weapon->StopFire();

}

void ULMAWeaponComponent::InitAnimNotify() 
{
   if (!ReloadMontage)
                return;

   const auto NotifiesEvents = ReloadMontage->Notifies;
   for (auto NotifyEvent : NotifiesEvents) 
   {
	auto ReloadFinish = Cast<ULMAReloadFinishedAnimNotify>(NotifyEvent.Notify);
	if (ReloadFinish) 
	{
		ReloadFinish->OnNotifyReloadFinished.AddUObject(this, &ULMAWeaponComponent::OnNotifyReloadFinished);
		break;

	}

   }

}

void ULMAWeaponComponent::OnNotifyReloadFinished(USkeletalMeshComponent *SkeletalMesh) 
{
   const auto Character = Cast<ACharacter>(GetOwner());
   if (Character->GetMesh() == SkeletalMesh) 
   {
        AnimReloading = false;

   }

}

bool ULMAWeaponComponent::CanReload() const 
{ 
	return !AnimReloading; 

}

void ULMAWeaponComponent::Reload() 
{
   if (!CanReload() || Weapon->IsCurrentClipFull())
        return;
   Weapon->ChangeClip();
   AnimReloading = true;
   ACharacter *Character = Cast<ACharacter>(GetOwner());
   Character->PlayAnimMontage(ReloadMontage);

}

void ULMAWeaponComponent::OnCurrentClipEmpty() 
{
	Reload();

}

bool ULMAWeaponComponent::GetCurrentWeaponAmmo(FAmmoWeapon &AmmoWeapon) const 
{
   if (Weapon) 
   {
        AmmoWeapon = Weapon->GetCurrentAmmoWeapon();
        return true;

   }
   return false;

}