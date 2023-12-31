// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LMABaseWeapon.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnCurrentClipEmpty)

class USkeletalMeshComponent;
class USoundWave;
class UNiagaraSystem;

USTRUCT(BlueprintType)
struct FAmmoWeapon 
{
  GENERATED_USTRUCT_BODY()

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Bullets;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  int32 Clips;

  UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
  bool Infinite;

};

UCLASS()
class LEAVEMEALONE_API ALMABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALMABaseWeapon();

	void Fire();
    void StopFire();
    void ChangeClip();

	bool IsCurrentClipFull() const;

	FOnCurrentClipEmpty OnCurrentClipEmpty;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Weapon")
    USkeletalMeshComponent *WeaponComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float TraceDistance = 1800.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FAmmoWeapon AmmoWeapon{30, 0, true};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    USoundWave *ShootWave;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    UNiagaraSystem *TraceEffect;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    FString TraceName = "Tracer";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon")
    float Damage = 25;

    void SpawnTrace(const FVector &TraceStart, const FVector &TraceEnd);

    void Shoot();

	void DecrementBullets();
    bool IsCurrentClipEmpty() const;
    
	void MakeDamage(const FHitResult &HitResult);

	FTimerHandle TimerHandle;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FAmmoWeapon GetCurrentAmmoWeapon() const { return CurrentAmmoWeapon; }

private:
	FAmmoWeapon CurrentAmmoWeapon;

};
