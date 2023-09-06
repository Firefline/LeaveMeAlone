// LeaveMeAlone Game by Netologiya. All Rights Reserved.


#include "LMADefaultCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/DecalComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "LeaveMeAlone/Components/LMAHealthComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/Engine.h"

// Sets default values
ALMADefaultCharacter::ALMADefaultCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
    SpringArmComponent->SetupAttachment(GetRootComponent());
    SpringArmComponent->SetUsingAbsoluteRotation(true);
    SpringArmComponent->TargetArmLength = ArmLength;
    SpringArmComponent->SetRelativeRotation(FRotator(YRotation, 0.0f, 0.0f));
    SpringArmComponent->bDoCollisionTest = false;
    SpringArmComponent->bEnableCameraLag = true;

    CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
    CameraComponent->SetupAttachment(SpringArmComponent);
    CameraComponent->SetFieldOfView(FOV);
    CameraComponent->bUsePawnControlRotation = false;

    HealthComponent = CreateDefaultSubobject<ULMAHealthComponent>("HealthComponent");

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

}

// Called when the game starts or when spawned
void ALMADefaultCharacter::BeginPlay()
{
	Super::BeginPlay();

    if (CursorMaterial) 
    {
        CurrentCursor = UGameplayStatics::SpawnDecalAtLocation(GetWorld(), CursorMaterial, CursorSize, FVector(0));

    }
	
    OnHealthChanged(HealthComponent->GetHealth());
    HealthComponent->OnDeath.AddUObject(this, &ALMADefaultCharacter::OnDeath);
    HealthComponent->OnHealthChanged.AddUObject(this, &ALMADefaultCharacter::OnHealthChanged);

}

// Called every frame
void ALMADefaultCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (HealthComponent->IsAlive()) 
    {
        RotationPlayerOnCursor();

    }


    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC) 
    {
        FHitResult ResultHit;
        PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
        float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
        SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
        if (CurrentCursor) 
        {
            CurrentCursor->SetWorldLocation(ResultHit.Location);
        }
    }

}

// Called to bind functionality to input
void ALMADefaultCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis("MoveForward", this, &ALMADefaultCharacter::MoveForward);
    PlayerInputComponent->BindAxis("MoveRight", this, &ALMADefaultCharacter::MoveRight);
    PlayerInputComponent->BindAction("ZoomIn", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::ZoomIn);
    PlayerInputComponent->BindAction("ZoomOut", EInputEvent::IE_Pressed, this, &ALMADefaultCharacter::ZoomOut);

}

void ALMADefaultCharacter::MoveForward(float Value) 
{
    AddMovementInput(GetActorForwardVector(), Value);

}
void ALMADefaultCharacter::MoveRight(float Value) 
{
    AddMovementInput(GetActorRightVector(), Value);

}

void ALMADefaultCharacter::ZoomIn()
{
    if (!Controller) return;

    float NewTargetArmLength = SpringArmComponent->TargetArmLength + ZoomStep * -1.0f;
    SpringArmComponent->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomLength, MaxZoomLength);

}

void ALMADefaultCharacter::ZoomOut()
{
    if (!Controller) return;

    float NewTargetArmLength = SpringArmComponent->TargetArmLength + ZoomStep;
    SpringArmComponent->TargetArmLength = FMath::Clamp(NewTargetArmLength, MinZoomLength, MaxZoomLength);

}

void ALMADefaultCharacter::OnDeath() 
{
    CurrentCursor->DestroyRenderState_Concurrent();

    PlayAnimMontage(DeathMontage);
    GetCharacterMovement()->DisableMovement();
    SetLifeSpan(5.0f);

    if (Controller) 
    {
        Controller->ChangeState(NAME_Spectating);

    }

}

void ALMADefaultCharacter::OnHealthChanged(float NewHealth) 
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("Health = %f"), NewHealth));

}

void ALMADefaultCharacter::RotationPlayerOnCursor() 
{
    APlayerController *PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (PC) 
    {
        FHitResult ResultHit;
        PC->GetHitResultUnderCursor(ECC_GameTraceChannel1, true, ResultHit);
        float FindRotatorResultYaw = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), ResultHit.Location).Yaw;
        SetActorRotation(FQuat(FRotator(0.0f, FindRotatorResultYaw, 0.0f)));
        if (CurrentCursor) 
        {
            CurrentCursor->SetWorldLocation(ResultHit.Location);

        }

    }

}