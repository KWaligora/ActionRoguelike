// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;
	
public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	UPROPERTY(EditAnywhere, Category = "Camera",  meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* SpringArmComp;
	UPROPERTY(EditAnywhere, Category = "Camera",  meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere)
	class USInteractionComponent* InteractionComp;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	void PrimaryAttack();
	void PrimaryInteract();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
