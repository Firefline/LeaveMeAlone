// LeaveMeAlone Game by Netologiya. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LMADefaultCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class ULMAHealthComponent;
class UAnimMontage;

UCLASS()
class LEAVEMEALONE_API ALMADefaultCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALMADefaultCharacter();

	UFUNCTION()
    ULMAHealthComponent *GetHealthComponent() const {	return HealthComponent;	};

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	USpringArmComponent *SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraComponent *CameraComponent;

	UPROPERTY()
    UDecalComponent *CurrentCursor = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    UMaterialInterface *CursorMaterial = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Cursor")
    FVector CursorSize = FVector(20.0f, 40.0f, 40.0f);

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components|Health")
    ULMAHealthComponent *HealthComponent;

    UPROPERTY(EditDefaultsOnly, Category = "Animation")
    UAnimMontage *DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
    float MinZoomLength = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
    float MaxZoomLength = 2500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera|Zoom")
    float ZoomStep = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Sprint")
    float SprintMultiplier;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Sprint")
    float CurrentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Sprint")
    float MaxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character|Sprint")
    float StaminaRegenMultiplier;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
    float YRotation = -75.0f;
	float ArmLength = 1400.0f;
    float FOV = 90.0f;

	void MoveForward(float Value);
    void MoveRight(float Value);
	void ZoomIn();
    void ZoomOut();

	void OnDeath();
    void OnHealthChanged(float NewHealth);

	void RotationPlayerOnCursor();

	void StartSprint();
    void StopSprint();
    void StaminaRegen();
    bool IsSprinting;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
