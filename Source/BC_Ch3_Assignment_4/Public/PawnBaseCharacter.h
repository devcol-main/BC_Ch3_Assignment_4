#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PawnBaseCharacter.generated.h"


// 미리 선언
// 전방 선언(Forward Declaration) 
class UCapsuleComponent;
class USkeletalMeshComponent;

class USpringArmComponent; // 스프링 암 관련 클래스 헤더
class UCameraComponent; // 카메라 관련 클래스 전방 선언




class UInputMappingContext;
class UInputAction;
// Enhanced Input에서 액션 값을 받을 때 사용하는 구조체
struct FInputActionValue;

UCLASS()
class BC_CH3_ASSIGNMENT_4_API APawnBaseCharacter : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APawnBaseCharacter();
	
	/*
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RandomActor|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="RandomActor|Components")
	TObjectPtr<UStaticMeshComponent> StaticMeshComp;	  
	 */
	
	// 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	USceneComponent* SceneRoot;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character")
	//UCapsuleComponent* CapsuleComp = nullptr;
	TObjectPtr<UCapsuleComponent> CapsuleComp = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Character")	
	USkeletalMeshComponent* SkeletalMeshComp = nullptr;
	
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

	FVector2D CurrentMoveInput;
	FVector2D CurrentLookInput;
	
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float NormalMoveSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Control")
	float LookSpeed; 		
	
	void Move(const FInputActionValue& Value);
	void StopMove(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	//
	

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	
	
};
