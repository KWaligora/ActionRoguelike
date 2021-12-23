#include "SCharacter.h"

#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "SMagicProjectile.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &ASCharacter::Ability1);
	
	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	FVector RightVector = FRotationMatrix(ControlRot).GetUnitAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

// Spawn projectile from right hand location
void ASCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_PrimaryAttack, this, &ASCharacter::PrimaryAttack_TimeElapsed, 0.2f);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	// Line Trace
	FHitResult Hit;
	FVector Start = CameraComp->GetComponentLocation();
	FVector End = CameraComp->GetForwardVector() * 10000 + Start;
	GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility);

	// Spawn data
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FRotator Rotation;
	if(IsValid(Hit.GetActor()))
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Red, true);
		Rotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, Hit.ImpactPoint);
	}
	else
	{
		Rotation = UKismetMathLibrary::FindLookAtRotation(HandLocation, End);
	}
	FTransform SpawnTM = FTransform(Rotation,HandLocation);
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	AActor* Projectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
}

void ASCharacter::Ability1()
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle_Ability1, this, &ASCharacter::Ability1_TimerElapse, 0.2f);
}

void ASCharacter::Ability1_TimerElapse()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	FTransform SpawnTM = FTransform(GetControlRotation(),HandLocation);
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(Ability1Class, SpawnTM, SpawnParams);
}

void ASCharacter::PrimaryInteract()
{
	InteractionComp->PrimaryInteract();
}



