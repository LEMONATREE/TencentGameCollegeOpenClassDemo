// Copyright Epic Games, Inc. All Rights Reserved.

#include "picCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Bome.h"
#include "Dog.h"
#include "EnermyAIController.h"
#include "TimerManager.h"
#include "BaseAnimInstance.h"
#include "BaseCharacterInterface.h"
#include "BlackHoleActor.h"
#include "NavigationSystem.h"
#include "picGameInstance.h"
ApicCharacter::ApicCharacter()
{
	//Set capsule size
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetGenerateOverlapEvents(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	//Set collision params
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionProfileName(UCollisionProfile::CustomCollisionProfileName);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetCollisionResponseToAllChannels(ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Overlap);
	GetMesh()->BodyInstance.bUseCCD = true;
	GetMesh()->bTraceComplexOnMove = true;
	/**Use playerController to control z-axis
	 * In this way, the player's forward orientation will be 
	 * consistent with the orientation of the playercontroller
	 */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;
	//Create camera and attach to boom to get view
	mySpringArm1 = CreateDefaultSubobject<USpringArmComponent>(TEXT("mySpringArm1"));
	mySpringArm1->SetupAttachment(GetRootComponent());
	mySpringArm1->SetUsingAbsoluteRotation(true);// Rotation of the character should not affect rotation of boom
	mySpringArm1->bDoCollisionTest = false;
	mySpringArm1->TargetArmLength = 600.f;
	mySpringArm1->SocketOffset = FVector(0.f,0.f,200.f);
	mySpringArm1->SetRelativeRotation(FRotator(0.f,180.f,0.f));//Rotate 180 around z-axis
	CameraBoom1 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera1"));
	CameraBoom1->SetupAttachment(mySpringArm1, USpringArmComponent::SocketName);
	CameraBoom1->bUsePawnControlRotation = true;

	mySpringArm2 = CreateDefaultSubobject<USpringArmComponent>(TEXT("mySpringArm2"));
	mySpringArm2->SetupAttachment(GetRootComponent());
	mySpringArm2->SetUsingAbsoluteRotation(true); 
	mySpringArm2->bDoCollisionTest = false;
	mySpringArm2->TargetArmLength = 600.f;
	mySpringArm2->SocketOffset = FVector(0.f, 0.f, 200.f);
	mySpringArm2->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	mySpringArm2->SetWorldRotation(FRotator(0.f,-90.f,0.f));
	CameraBoom2 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera2"));
	CameraBoom2->SetupAttachment(mySpringArm2,USpringArmComponent::SocketName);
	CameraBoom2->bUsePawnControlRotation = true;

	mySpringArm3 = CreateDefaultSubobject<USpringArmComponent>(TEXT("mySpringArm3"));
	mySpringArm3->SetupAttachment(GetRootComponent());
	mySpringArm3->SetUsingAbsoluteRotation(true);
	mySpringArm3->bDoCollisionTest = false;
	mySpringArm3->TargetArmLength = 600.f;
	mySpringArm3->SocketOffset = FVector(0.f, 0.f, 0.f);
	mySpringArm3->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	mySpringArm3->SetWorldRotation(FRotator(-90.f, -90.f, 0.f));
	CameraBoom3 = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera3"));
	CameraBoom3->SetupAttachment(mySpringArm3, USpringArmComponent::SocketName);
	CameraBoom3->bUsePawnControlRotation = true;
	//Push into camera array
	MyCamera.Push(CameraBoom1);
	MyCamera.Push(CameraBoom2);
	MyCamera.Push(CameraBoom3);
	//Default to use Camera1
	mySpringArm1->SetAutoActivate(true);
	CameraBoom1->SetAutoActivate(true);
	mySpringArm2->SetAutoActivate(true);
	CameraBoom2->SetAutoActivate(false);
	mySpringArm3->SetAutoActivate(true);
	CameraBoom3->SetAutoActivate(false);
	//Create childcomp to get firstperson view
	ChildComp = CreateDefaultSubobject<UChildActorComponent>(TEXT("ChildComp"));
	ChildComp->SetupAttachment(RootComponent);
	//Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Face in the direction we are moving..
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->GravityScale = 2.f;
	GetCharacterMovement()->AirControl = 0.80f;
	GetCharacterMovement()->JumpZVelocity = 1000.f;
	GetCharacterMovement()->GroundFriction = 3.f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;
	//Initial blueprint path
	EnermyBP_Path = TEXT("Blueprint'/Game/SideScrollerCPP/AI/EnermyCharacter_BP.EnermyCharacter_BP_C'");
	WeaponBP_Path = TEXT("Blueprint'/Game/SwordAnimsetPro/Models/Sword/WeaponActor_BP.WeaponActor_BP_C'");
	//Mark to decide damage causer etc.
	IsPlayer = true;
	//Create ComboWidgetComponent and bind to blueprint
	ComboWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Combo"));
	ComboWidgetComp->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);//Attach to show
	ComboWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));//Avoid unnecessary collision
	ComboWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	ComboWidgetBP_Path = TEXT("WidgetBlueprint'/Game/SideScrollerCPP/UI/Combo.Combo_C'");
	ComboWidgetBP_Class = LoadBP<UUserWidget>(ComboWidgetBP_Path);
	ComboWidgetComp->SetWidgetClass(ComboWidgetBP_Class);
	//Other params
	MaxExistEnermyNum = 3;
	ExtraLife = 2;
	MaxEnergy = 600.f;
	CurrentEnergy = MaxEnergy;
	RevivePoint = FVector(1100.f, -7000.f, 100.f);
}
// Input
void ApicCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Speed", IE_Pressed, this, &ApicCharacter::SpeedUp);
	PlayerInputComponent->BindAction("Speed", IE_Released, this, &ApicCharacter::SpeedDown);
	PlayerInputComponent->BindAxis("MoveRight", this, &ApicCharacter::MoveRight);
	PlayerInputComponent->BindAxis("MoveForward", this, &ApicCharacter::MoveForward);
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ApicCharacter::JumpStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ApicCharacter::JumpStopped);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("GetFirstPersonView", IE_Pressed, this, &ApicCharacter::GetFirstPersonView);
	PlayerInputComponent->BindAction("GetThirdPersonView", IE_Pressed, this, &ApicCharacter::GetThirdPersonView);
	PlayerInputComponent->BindAction("Bullet", IE_Pressed, this, &ApicCharacter::BulletTime);
	PlayerInputComponent->BindAction("Skill_W", IE_Pressed, this, &ApicCharacter::Skill_W);
	PlayerInputComponent->BindAction("Skill_E", IE_Pressed, this, &ApicCharacter::Skill_E);
	PlayerInputComponent->BindAction("Skill_R", IE_Pressed, this, &ApicCharacter::Skill_R);
	//只有返回值为void的函数才能BindAction？
}
/** GetView */
void ApicCharacter::GetFirstPersonView()
{
	APlayerController* MainController = Cast<APlayerController>(Controller);
	if (MainController && IsThirdPersonView() && ChildComp && ChildComp->GetChildActor())
	{
		MainController->SetViewTarget(ChildComp->GetChildActor());
	}
	else if (MainController && !IsThirdPersonView() && (this != nullptr))
	{ 
		MainController->SetViewTargetWithBlend(this);
	}
}
void ApicCharacter::GetThirdPersonView()
{
	MyCamera[NowCamera]->SetActive(false);
	NowCamera++;
	NowCamera = NowCamera % (MyCamera.Num());
	MyCamera[NowCamera]->SetActive(true);
}
bool ApicCharacter::IsThirdPersonView()
{
	APlayerController* MainController = Cast<APlayerController>(Controller);
	return (MainController->GetViewTarget() == this);
}
/** Move */
void ApicCharacter::MoveRight(float Value)
{
	FVector Direction = IsThirdPersonView() ? GetThirdPersonDirection(TEXT("Right")) : GetFirstPersonDirection(TEXT("Right"));
	AddMovementInput(Direction, Value);
}
void ApicCharacter::MoveForward(float Value)
{
	FVector Direction = IsThirdPersonView() ? GetThirdPersonDirection(TEXT("Forward")) : GetFirstPersonDirection(TEXT("Forward"));
	AddMovementInput(Direction, Value);
}
FVector ApicCharacter::GetFirstPersonDirection(FString DirectName)
{
	//第一人称是以角色当前的正方向为forward方向，所以正方向一直在变
	FVector Direction = GetActorForwardVector();
	if (DirectName == TEXT("Forward"))
		return Direction;
	else if (DirectName == TEXT("Right"))
		return FVector(-Direction.Y, Direction.X, 0.f);//可以尝试GetActorRightVector?
	else
		return FVector(0.f);
}
FVector ApicCharacter::GetThirdPersonDirection(FString DirectName)
{
	//第三人称的正方向不变//获取camera朝向，正向就是forward方向
	//注意ue中是左手系，顺时针旋转的正方向//当前激活的camera-->被记录在NowCamera中
	FVector Direction = FVector(0.f);
	if (NowCamera == 0 || NowCamera == 1)//正视图或者左视图
	{
		auto TmpCamera = MyCamera[NowCamera];
		Direction = TmpCamera->GetForwardVector();//正方向
		if (DirectName == TEXT("Forward"))
			return Direction;
		else if (DirectName == TEXT("Right"))//右方向
			return FVector(-Direction.Y, Direction.X, 0.f);
		else
			return FVector(0.f);
	}
	else
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);
		//因为输入z/x会改变Yaw，因此俯视图需要对CameraBoom做相同旋转->需要绕着x轴旋转（即改变Roll）
		CameraBoom3->SetRelativeRotation(FRotator(0.f, 0.f, Rotation.Yaw));//按z是顺时针旋转
		if (DirectName == TEXT("Forward"))
			return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		else if (DirectName == TEXT("Right"))
			return FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		else
			return Direction;
	}
}
/** Jump */
void ApicCharacter::JumpStarted(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	Jump();
}
void ApicCharacter::JumpStopped(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	StopJumping();
}
/** SpeedUp */
void ApicCharacter::SpeedUp()
{
	if (IsSpeedUp == false)//不要重复生成定时器
	{
		SetSpeedParam(true, BaseSpeed + CurrentEnergy);
	}
}
void ApicCharacter::SpeedDown()
{
	if (IsSpeedUp == true)
	{
		SetSpeedParam(false, BaseSpeed);
	}
}
void ApicCharacter::SetSpeedParam(bool SpeedUpState, float MoveSpeed)
{
	IsSpeedUp = SpeedUpState;
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
	StopTimerHandle(SpeedUpHandle);
	float TickTime = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(SpeedUpHandle, this, &ApicCharacter::UpdateEnergy, TickTime, true);
}
void ApicCharacter::UpdateEnergy()
{
	//事件触发代替tick
	if (IsSpeedUp)
	{
		CurrentEnergy = (CurrentEnergy > 0.f) ? (CurrentEnergy - 0.5f) : 0.f;//消耗能量，最小为0
		GetCharacterMovement()->MaxWalkSpeed = BaseSpeed + CurrentEnergy;
	}
	else
	{
		CurrentEnergy = (CurrentEnergy < MaxEnergy) ? (CurrentEnergy + 1.f) : MaxEnergy;//恢复能量，最大为BaseSpeed
	}
	if (CurrentEnergy == 0 || CurrentEnergy == MaxEnergy)//能量消耗完毕或者已经满了
	{
		StopTimerHandle(SpeedUpHandle);//不再更新
	}
}
/** SpawnEnermy */
void ApicCharacter::SpawnEnermy()
{
	//spawn生成的enermy不自带controller
	FVector PlayerLocation = GetActorLocation();
	FVector SpawnLocation = PlayerLocation;
	FRotator SpawnRotation(0.f);

	FNavLocation TargetNavLocation;
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	
	if (NavSystem && NavSystem->GetRandomPointInNavigableRadius(PlayerLocation, 500.f, TargetNavLocation))
	{
		UE_LOG(LogTemp, Warning, TEXT("GetNavigation!!"));
		SpawnLocation = TargetNavLocation.Location;
		SpawnLocation.Z = PlayerLocation.Z;
	}
	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, PlayerLocation);
	SpawnRotation.Yaw = Turn.Yaw;
	//SpawnUsefulActor(SpawnLocation,SpawnRotation);
	AEnermyCharacter* ActorRef = BaseSpawnActor<AEnermyCharacter>(EnermyBP_Path, SpawnLocation, SpawnRotation);
	if (ActorRef)
	{
		ActorRef->SpawnDefaultController();
	}
}
void ApicCharacter::DelaySpawnEnermy(float DelayTime)
{
	if (DelayTime <= 0.f)
	{
		SpawnEnermy();
	}
	else
	{
		FTimerHandle SpawnHandle;
		//FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ApicCharacter::SpawnBlackHole);
		//有返回值时不能用settimer？ 如果返回类型为ABlackHoleActor*就会报错
		GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ApicCharacter::SpawnEnermy, DelayTime);//延时一段时间后执行	
	}
}
/** Combo */
void ApicCharacter::BindComboWidget()
{
	if (ComboWidgetBP_Class)
	{
		ComboWidget = CreateWidget<UComboWidget>(GetWorld()->GetGameInstance(), ComboWidgetBP_Class);
		if (ComboWidget && ComboWidgetComp)
		{
			ComboWidgetComp->SetWidget(ComboWidget);
		}
		ComboWidget->HideCombo();//默认隐藏
	}
}
void ApicCharacter::UpdateComboWidget()
{
	if (ComboWidget && ComboWidgetComp)
	{
		ComboWidget->UpdateCombo(ComboCount);
	}
}
void ApicCharacter::UpdateComboWidgetWithTime()
{
	float TickTime = GetWorld()->GetDeltaSeconds();
	GetWorld()->GetTimerManager().SetTimer(ComboUpdateHandle, this, &ApicCharacter::UpdateComboWidget, TickTime, true);//延时一段时间后执行	
}
void ApicCharacter::ResetComboWidget()//重置回默认状态
{
	StopTimerHandle(ComboUpdateHandle);
	ComboCount = 0;
	ComboWidget->HideCombo();
}
void ApicCharacter::DelayResetComboWidget()
{
	float DelayTime = 5.f;
	GetWorld()->GetTimerManager().SetTimer(ComboResetHandle, this, &ApicCharacter::ResetComboWidget, DelayTime);//若Combo持续为0，则延时一段时间后消失	
}
/** Skill */
void ApicCharacter::BulletTime()
{
	UWorld* World = GetWorld();
	if (World)
	{
		if (this->CustomTimeDilation == 1)
		{
			this->CustomTimeDilation = 0.3;
			SetTimeFlow<ABome>(0.05f);
			SetTimeFlow<ADog>(0.05f);
			SetTimeFlow<AEnermyCharacter>(0.05f);
		}
		else
		{
			this->CustomTimeDilation = 1;
			SetTimeFlow<ABome>(1.f);
			SetTimeFlow<ADog>(1.f);
			SetTimeFlow<AEnermyCharacter>(1.f);
		}
	}
}
void ApicCharacter::Skill_W()
{
	UE_LOG(LogTemp, Warning, TEXT("Skill_W1!"));
	Weapon->SetDamageCoeff(1.f);
	UE_LOG(LogTemp, Warning, TEXT("PlayMont:%d!"),IsPlayingMontage);
	float t = CastSkill(this, Skill_Q_Montage, 2.0f);
}
void ApicCharacter::Skill_E()
{
	Weapon->SetDamageCoeff(2.f);
	float t = CastSkill(this, Skill_E_Montage, 2.0f);
}
void ApicCharacter::Skill_R()
{
	Weapon->SetDamageCoeff(1.5f);
	float t = CastSkill(this, Skill_R_Montage, 2.0f);
}
void ApicCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindComboWidget();
	Weapon = BaseSpawnActor<AWeaponActor>(WeaponBP_Path, FVector(0.f), FRotator(0.f));
	if (Weapon != nullptr)
	{
		Weapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
		Weapon->SetIsPlayer(true);
		UE_LOG(LogTemp, Warning, TEXT("Weapon_initialize!"));
		// 附加武器，保持位置
		//Weapon->GetWeaponMesh()->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon"));
	}
	UpicGameInstance* GI = Cast<UpicGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		GI->SetGameBeginTime(FPlatformTime::Seconds());
		MaxExistEnermyNum = GI->GetCurrentMode() + 1;
		if (GI->GetCurrentMap() == 1)//初始化玩家生命血量
		{
			EnermyNumToSpawn = MaxExistEnermyNum;
			ToSpawnEnermy = true;
		}
		else
		{
			GLog->Log("Spawn Boss");
		}
	}
	UpdateLevel(this);//初始化玩家生命血量

}
void ApicCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeathCheck(this);
	UpdateWeaponLocationRotation(this);//先关闭更新用来测试回旋镖
	if (ToSpawnEnermy)
	{
		while (EnermyNumToSpawn)
		{
			EnermyNumToSpawn--;
			DelaySpawnEnermy(2.f);
		}
		ToSpawnEnermy = false;
	}
}

void ApicCharacter::SetViewOnBlackHole(ABlackHoleActor* BlackHole)
{
	//将视角移到blackhole上
	APlayerController* MainController = Cast<APlayerController>(Controller);
	if (MainController)
	{
		MainController->SetViewTargetWithBlend(BlackHole, 1.f);
	}
	//隔3s复原视角	
	DelayResetViewFromBlackHole(3.f);
}
void ApicCharacter::DelaySetViewOnBlackHole(ABlackHoleActor* BlackHole, float DelayTime)
{
	FTimerHandle OutHandle;
	FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ApicCharacter::SetViewOnBlackHole, BlackHole);
	GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, DelayTime, false);
}
void ApicCharacter::ResetViewFromBlackHole()
{
	APlayerController* MainController = Cast<APlayerController>(Controller);
	if (MainController)
	{
		MainController->SetViewTargetWithBlend(this, 1.f);
	}
}
void ApicCharacter::DelayResetViewFromBlackHole(float DelayTime)
{
	FTimerHandle OutHandle;
	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &ApicCharacter::ResetViewFromBlackHole, DelayTime);
}