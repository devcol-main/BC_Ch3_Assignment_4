
#include "MainCharacter.h"

// 카메라, 스프링 암 실제 구현이 필요한 경우라서 include
// 전방 선언(Forward Declaration) 한것 여기서 (실질적으로 사용하는곳) 포함
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AMainCharacter::AMainCharacter()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	
	
	// (1) 스프링 암 생성
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	// 스프링 암을 루트 컴포넌트 (CapsuleComponent)에 부착
	SpringArmComp->SetupAttachment(RootComponent);
	// 캐릭터와 카메라 사이의 거리 기본값 300으로 설정
	SpringArmComp->TargetArmLength = 300.0f;  
	// 컨트롤러 회전에 따라 스프링 암도 회전하도록 설정
	SpringArmComp->bUsePawnControlRotation = true;  
	
	
	//

	// (2) 카메라 컴포넌트 생성
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	// 스프링 암의 소켓 위치에 카메라를 부착
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	// 카메라는 스프링 암의 회전을 따르므로 PawnControlRotation은 꺼둠
	CameraComp->bUsePawnControlRotation = false;
	
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

