// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "../Global/GlobalGameInstance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	JumpMaxHoldTime = 0.0f;

	// 생성자에서 컴포넌트를 만드는 방법은 다음의 함수를 사용하면 된다.

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WeaponMesh"));

	// 어떤 다른 부모에게 나를 붙인다는 이야기가 됩니다.
	WeaponMesh->SetupAttachment(GetMesh(), TEXT("B_R_Weapon"));
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	JumpMaxCount = 2;

	GetWorld()->GetAuthGameMode();

	UGlobalGameInstance* Inst = GetGameInstance<UGlobalGameInstance>();

	WeaponArrays.Add(GetGameInstance<UGlobalGameInstance>()->GetMesh(TEXT("Weapon")));
	WeaponArrays.Add(GetGameInstance<UGlobalGameInstance>()->GetMesh(TEXT("Cube")));

	WeaponMesh->SetStaticMesh(WeaponArrays[0]);
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//UE_LOG(LogTemp, Error, TEXT("%d"), AniState);
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	static bool bBindingsAdded = false;
	if (!bBindingsAdded)
	{
		bBindingsAdded = true;

		// 여기의 내용은 뭐냐?
		// DefaultPawn_MoveForward 추가되는것 뿐
		// 축매핑만 하고 있스니다.
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLookUp", EKeys::MouseY, -1.f));

		UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerJumpAxis", EKeys::E, -1.f));


		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerAttack"), EKeys::LeftMouseButton));
		UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping(TEXT("PlayerJumpAction"), EKeys::SpaceBar));
	}

	// 키와 함수를 연결합니다.
	// 이 키가 눌리면 이 함수를 실행시켜줘인데.
	// 축일때는 일단 계속 실행시켜줘.
	PlayerInputComponent->BindAxis("PlayerMoveForward", this, &ATPSCharacter::MoveForward);
	PlayerInputComponent->BindAxis("PlayerMoveRight", this, &ATPSCharacter::MoveRight);
	PlayerInputComponent->BindAxis("PlayerTurn", this, &ATPSCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("PlayerTurnRate", this, &ATPSCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("PlayerLookUp", this, &ATPSCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("PlayerLookUpRate", this, &ATPSCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("PlayerJumpAxis", this, &ATPSCharacter::JumpAxis);

	PlayerInputComponent->BindAction("PlayerAttack", EInputEvent::IE_Pressed, this, &ATPSCharacter::AttackAction);
	PlayerInputComponent->BindAction("PlayerJumpAction", EInputEvent::IE_Pressed, this, &ATPSCharacter::JumpAction);
}

void ATPSCharacter::MoveRight(float Val)
{
	if (AniState == TPSAniState::Attack || AniState == TPSAniState::Jump)
	{
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			FRotator const ControlSpaceRot = Controller->GetControlRotation();
			// transform to world space and add it
			// 현재 내 회전을 가져와서 y축에 해당하는 축벡터를 얻어오는 겁니다.
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), Val);

			AniState = Val > 0.f ? TPSAniState::RightMove : TPSAniState::LeftMove;
			return;
		}
	}
	else
	{
		if (AniState == TPSAniState::RightMove || AniState == TPSAniState::LeftMove)
		{
			AniState = TPSAniState::Idle;
		}
	}
}

void ATPSCharacter::MoveForward(float Val)
{
	if (AniState == TPSAniState::Attack || AniState == TPSAniState::Jump)
	{
		return;
	}

	if (Val != 0.f)
	{
		if (Controller)
		{
			// 컨트롤러는 기본적으로
			// charcter 하나씩 붙어 있습니다.
			FRotator const ControlSpaceRot = Controller->GetControlRotation();

			// 이건 방향일 뿐입니다.
			// transform to world space and add it
			AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), Val);
			// 탑뷰게임이면 다르게 나오게 되는데.
			// 지금은 TPS를 하고 있기 때문에 컨트롤러의 회전이나 액터의 회전이나 같아요.
			// AddMovementInput(GetActorForwardVector(), Val);

			AniState = Val > 0.f ? TPSAniState::ForwardMove : TPSAniState::BackwardMove;
			return;
		}
	}
	else
	{
		if (AniState == TPSAniState::ForwardMove || AniState == TPSAniState::BackwardMove)
		{
			AniState = TPSAniState::Idle;
		}
	}

	// 이런 느낌의 함수 즉 static함수를 의미한다.
	// AEGLOBAL::DebugPrint("AAAAAAA");
}


void ATPSCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}

void ATPSCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds() * CustomTimeDilation);
}


void ATPSCharacter::JumpAxis(float Rate)
{
	if (0.0f == Rate)
	{
		if (true == AxisJump)
		{
			StopJumping();
			AxisJump = false;
		}
		return;
	}

	AxisJump = true;
	Jump();
}

void ATPSCharacter::JumpAction()
{
	// JumpCurrentCount
	UE_LOG(LogTemp, Log, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, JumpCurrentCount);
	Jump();

	AniState = TPSAniState::Jump;
}

void ATPSCharacter::AttackAction()
{
	// 무브먼트 컴포넌트를 통해서 한다.
	// GetMovementComponent()

	// PlayMontage();

	AniState = TPSAniState::Attack;
}