
#include "AircraftPawnBaseCharacter.h"

#include "Components/CapsuleComponent.h"
#include "Components/StaticMeshComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h" 

#include "DrawDebugHelpers.h"

// Sets default values
AAircraftPawnBaseCharacter::AAircraftPawnBaseCharacter()
{
	InitialSetup();

}

void AAircraftPawnBaseCharacter::InitialSetup()
{
	
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));	
	SetRootComponent(CapsuleComp);
	
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	StaticMeshComponent->SetupAttachment(CapsuleComp);
	//
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(RootComponent);	
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	
	//
	StaticMeshComponent_Ground = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent_Ground"));
	StaticMeshComponent_Ground->SetupAttachment(RootComponent);
	
	//
	// Physics Setting
	CapsuleComp->SetSimulatePhysics(false);
	StaticMeshComponent->SetSimulatePhysics(false);
	
	// Set Variable
	NormalMoveSpeed = 1000.0f;	
	LookSpeed = 100.0f;
	
	
	StaticMeshComponent_Ground->SetVisibility(true);
}

// Called when the game starts or when spawned
void AAircraftPawnBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
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
void AAircraftPawnBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	const bool bHasLookInput =
		!FMath::IsNearlyZero(CurrentLookInput.X) ||
	!FMath::IsNearlyZero(CurrentLookInput.Y) ||
	!FMath::IsNearlyZero(CurrentLookInput.Z);
	
	const bool bHasMoveInput =
	!FMath::IsNearlyZero(CurrentMoveInput.X) ||
	!FMath::IsNearlyZero(CurrentMoveInput.Y) ||
		!FMath::IsNearlyZero(CurrentMoveInput.Z);
	
	
	bIsGrounded = CheckGrounded();
	
	if (!bIsGrounded)
	{
		if (GEngine)
		{ 
			GEngine->AddOnScreenDebugMessage(
					-1, 0.5f, FColor::Green, 
					TEXT("AIR"));
		}
		
		AddActorWorldOffset(FVector(0.0f, 0.0f, Gravity) * DeltaTime, true);
		
		if (bHasMoveInput)
		{
			const FVector LocalMoveOffset =
				FVector(CurrentMoveInput.X, CurrentMoveInput.Y, 
					CurrentMoveInput.Z * UpMultiplier)
				* NormalMoveSpeed * AirMoveSpeed
				* DeltaTime;

			AddActorLocalOffset(LocalMoveOffset);
		
		}	
	}
	else
	{	
		
		if (GEngine)
		{ 
			GEngine->AddOnScreenDebugMessage(
					-1, 0.5f, FColor::Blue, 
					TEXT("GROUND"));
		}

		AddActorWorldOffset(FVector(0));		
		
		if (bHasMoveInput)
		{
			FVector LocalMoveOffset =
				FVector(CurrentMoveInput.X, CurrentMoveInput.Y, 
					CurrentMoveInput.Z * UpMultiplier)
				* NormalMoveSpeed
				* DeltaTime;
			
			LocalMoveOffset.Z = FMath::Max(LocalMoveOffset.Z, 0.0f);
			
			AddActorLocalOffset(LocalMoveOffset);
		
		}
	}	

	// loock
	if (bHasLookInput)
	{
		
		FRotator SpringArmRot = SpringArmComp->GetRelativeRotation();

		SpringArmRot.Pitch += CurrentLookInput.Y * LookSpeed * DeltaTime;
		SpringArmRot.Yaw += CurrentLookInput.X * LookSpeed * DeltaTime;
		SpringArmRot.Roll += CurrentLookInput.Z * (LookSpeed* 2.0f) * DeltaTime;

		SpringArmRot.Pitch = FMath::Clamp(SpringArmRot.Pitch, -60.0f, 30.0f);
		SpringArmRot.Yaw = FMath::Clamp(SpringArmRot.Yaw, -60.0f, 60.0f);

		//SpringArmComp->SetRelativeRotation(SpringArmRot);
		AddActorLocalRotation(SpringArmRot);
		

	}
}

// Called to bind functionality to input
void AAircraftPawnBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
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
				&AAircraftPawnBaseCharacter::Move
			);
			
			EnhancedInputComponent->BindAction(
				MoveAction,
				ETriggerEvent::Completed,
				this,
				&AAircraftPawnBaseCharacter::StopMove
			);
		}
		

		if (LookAction)
		{
			EnhancedInputComponent->BindAction(
				LookAction,
				ETriggerEvent::Triggered,
				this,
				&AAircraftPawnBaseCharacter::Look
			);
			
			EnhancedInputComponent->BindAction(
				LookAction,
				ETriggerEvent::Completed,
				this,
				&AAircraftPawnBaseCharacter::StopLook
			);
			
			
		}
	}	

}



void AAircraftPawnBaseCharacter::Move(const FInputActionValue& Value)
{
	if (!Controller) return;
	
	//CurrentMoveInput = Value.Get<FVector2D>();
	CurrentMoveInput = Value.Get<FVector>();
	
}

void AAircraftPawnBaseCharacter::StopMove(const FInputActionValue& Value)
{
	CurrentMoveInput = FVector::ZeroVector;
}

void AAircraftPawnBaseCharacter::StopLook(const FInputActionValue& Value)
{
	CurrentLookInput = FVector::ZeroVector;
}

void AAircraftPawnBaseCharacter::Look(const FInputActionValue& Value)
{
	CurrentLookInput = Value.Get<FVector>();
	
}


bool AAircraftPawnBaseCharacter::CheckGrounded()
{

	FVector StartLocation = GetActorLocation();
	FVector DownVector = -GetActorUpVector(); 
    
	float TraceDistance = 50.f; 
	FVector EndLocation = StartLocation + (DownVector * TraceDistance);

	float CollisionRadius = 40.f;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(CollisionRadius);

	
	FHitResult HitResult;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepSingleByChannel(
		HitResult,
		StartLocation,
		EndLocation,
		FQuat::Identity,
		ECC_Visibility,
		SphereShape,
		QueryParams
	);

	// [선택 사항] 에디터에서 충돌 영역을 눈으로 확인하고 싶을 때 사용
	DrawDebugSphere(GetWorld(), EndLocation, CollisionRadius, 12, FColor::Orange, false, -1.f);

	StaticMeshComponent_Ground->SetVisibility(bHit);
	
	return bHit;
}

