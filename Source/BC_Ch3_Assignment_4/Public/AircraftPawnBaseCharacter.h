// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "AircraftPawnBaseCharacter.generated.h"


class UCapsuleComponent;
class UStaticMeshComponent;
class USpringArmComponent; 
class UCameraComponent; 
class UInputMappingContext;
class UInputAction;

struct FInputActionValue;

UCLASS()
class BC_CH3_ASSIGNMENT_4_API AAircraftPawnBaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AAircraftPawnBaseCharacter();

protected:
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
protected:
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//TObjectPtr<USceneComponent> SceneRoot = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")	
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")	
	UStaticMeshComponent* StaticMeshComponent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")	
	UStaticMeshComponent* StaticMeshComponent_Ground = nullptr;
	
	
	// Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	USpringArmComponent* SpringArmComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera")
	UCameraComponent* CameraComp = nullptr;
	

	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAction;
	
	//
	//Ground Check
	UPROPERTY(EditAnywhere, Category = "GroundCheck")
	float GroundCheckDistance = 10.0f;
	UPROPERTY(EditAnywhere, Category = "GroundCheck")
	float GroundCheckRadius = 30.0f;
	UPROPERTY(VisibleAnywhere, Category = "GroundCheck")
	bool bIsGrounded = false;
	
	//

	//FVector2D CurrentMoveInput;
	FVector CurrentMoveInput;
	FVector CurrentLookInput;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float Gravity = -980.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float UpMultiplier = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float NormalMoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float AirMoveSpeed = 0.5f;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LookSpeed; 		
	
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StopLook(const FInputActionValue& Value);
	
	UFUNCTION(BlueprintCallable)
	bool CheckGrounded();
private:
	void InitialSetup();
};
