// Fill out your copyright notice in the Description page of Project Settings.

#include "PawnBaseCharacter.h"

//
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
//#include "InputActionValue.h"

//

// 카메라, 스프링 암 실제 구현이 필요한 경우라서 include
// 전방 선언(Forward Declaration) 한것 여기서 (실질적으로 사용하는곳) 포함
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h" 



// Sets default values
APawnBaseCharacter::APawnBaseCharacter()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	/*
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	StaticMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComp->SetupAttachment(SceneRoot); 
	 */
    
	//SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	//SetRootComponent(SceneRoot);
	
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	//CapsuleComp->SetupAttachment(SceneRoot);
	SetRootComponent(CapsuleComp);
	
	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
	SkeletalMeshComp->SetupAttachment(RootComponent);
	
	// 현재  CapsuleComp을 RootComponent 으로 해서 동일하지만 확실하게 하기 위해서
	//SkeletalMeshComp->SetupAttachment(CapsuleComp); 
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);
	
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	
	// Physics Setting
	CapsuleComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetSimulatePhysics(false);
	
	//
	NormalMoveSpeed = 1000.0f;
	//GetCharacterMovement()->MaxWalkSpeed = NormalMoveSpeed;
	LookSpeed = 100.0f;
	
	
	
	// SetActorLocation(StartLocation);
}

// Called when the game starts or when spawned
void APawnBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
		
	// TODO: 현재 Pawn을 소유한 Controller 가져오기
	APlayerController* PlayerController = Cast<APlayerController>(GetController());

	if (PlayerController)
	{
		// TODO: LocalPlayer 가져오기
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();

		if (LocalPlayer)
		{
			// Local Player에서 EnhancedInputLocalPlayerSubsystem을 획득
			// UEnhancedInputLocalPlayerSubsystem: 입력 시스템을 관리 (IMC 추가 혹은 삭제하는 역할)
			UEnhancedInputLocalPlayerSubsystem* Subsystem =
				LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();

			if (Subsystem && InputMappingContext)
			{
				// Subsystem을 통해 우리가 할당한 IMC를 활성화
				// 우선순위(Priority)는 0이 가장 높은 우선순위
				Subsystem->AddMappingContext(InputMappingContext, 0);
			}
		}
	}
}

// Called every frame
void APawnBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
	
	// 깔끔하게 한번 정리해봄
	const bool bHasMoveInput =
	!FMath::IsNearlyZero(CurrentMoveInput.X) ||
	!FMath::IsNearlyZero(CurrentMoveInput.Y);

	if (bHasMoveInput)
	{
		const FVector LocalMoveOffset =
			FVector(CurrentMoveInput.X, CurrentMoveInput.Y, 0.0f)
			* NormalMoveSpeed
			* DeltaTime;

		AddActorLocalOffset(LocalMoveOffset);
	}
	
	// 그냥 내가 간편하게	
	
	if (!FMath::IsNearlyZero(CurrentLookInput.X) ||
	!FMath::IsNearlyZero(CurrentLookInput.Y))
	{
		FRotator SpringArmRot = SpringArmComp->GetRelativeRotation();

		SpringArmRot.Pitch += CurrentLookInput.Y * LookSpeed * DeltaTime;
		SpringArmRot.Yaw += CurrentLookInput.X * LookSpeed * DeltaTime;

		SpringArmRot.Pitch = FMath::Clamp(SpringArmRot.Pitch, -60.0f, 30.0f);
		SpringArmRot.Yaw = FMath::Clamp(SpringArmRot.Yaw, -60.0f, 60.0f);

		SpringArmComp->SetRelativeRotation(SpringArmRot);
	}
	
}

// Called to bind functionality to input
void APawnBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInputComponent =
	Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	if (EnhancedInputComponent)
	{
		if (MoveAction)
		{
			EnhancedInputComponent->BindAction(
				MoveAction,
				ETriggerEvent::Triggered,
				this,
				&APawnBaseCharacter::Move
			);
			
			EnhancedInputComponent->BindAction(
				MoveAction,
				ETriggerEvent::Completed,
				this,
				&APawnBaseCharacter::StopMove
			);
		}
		

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				this,
				&APawnBaseCharacter::Look
			);
		}
	}	
}

void APawnBaseCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	CurrentMoveInput = Value.Get<FVector2D>();
	
	
	/*
	// 원래 일반적인 방식
	
	if (!Controller) return;
	
	// Value는 Axis2D로 설정된 IA_Move의 입력값 (WASD)을 담고 있음
	// 예) (X=1, Y=0) → 전진 / (X=-1, Y=0) → 후진 / (X=0, Y=1) → 오른쪽 / (X=0, Y=-1) → 왼쪽
	const FVector2D MoveInput = Value.Get<FVector2D>();

	// IsNearlyZero
	// 부동소수점들은 딱 0으로 안 떨어 질 수도 있기 때문에 작은 오차들은 0으로 처리 하기 위한 함수
	if (!FMath::IsNearlyZero(MoveInput.X))
	{		
		// 캐릭터가 바라보는 방향(정면)으로 X축 이동
		AddMovementInput(GetActorForwardVector(), MoveInput.X);
	}

	if (!FMath::IsNearlyZero(MoveInput.Y))
	{
		// 캐릭터의 오른쪽 방향으로 Y축 이동
		AddMovementInput(GetActorRightVector(), MoveInput.Y);
	}
	*/
}

void APawnBaseCharacter::StopMove(const FInputActionValue& Value)
{
	CurrentMoveInput = FVector2D::ZeroVector;
}

void APawnBaseCharacter::Look(const FInputActionValue& Value)
{
	CurrentLookInput = Value.Get<FVector2D>();
	
	
	/*
	// 원래 일반적인 방식
	 
	// 마우스의 X, Y 움직임을 2D 축으로 가져옴
	FVector2D LookInput = Value.Get<FVector2D>();

	// X는 좌우 회전 (Yaw), Y는 상하 회전 (Pitch)
	// 좌우 회전
	AddControllerYawInput(LookInput.X);
	
	// 상하 회전
	// IA에서 반전해둔 상태
	// 여기엔 추후 변경 가능한 옵션 추가해주자
	AddControllerPitchInput(LookInput.Y);
	
	*/
}

