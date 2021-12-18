// Fill out your copyright notice in the Description page of Project Settings.
#include "EnermyCharacter.h"
#include "EnermyAIController.h"//头文件循环引用改放在cpp中
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
#include "EnermyAnimInstance.h"
#include "BaseCharacterInterface.h"
#include "picGameInstance.h"

AEnermyCharacter::AEnermyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	AIControllerClass = AEnermyAIController::StaticClass();
	/** Sense */
	Sight = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Sensing"));
	GetCharacterMovement()->bUseRVOAvoidance = true;//开启RVO避让
	/** BehaviorTree */
	BehaviorTree_Path = TEXT("BehaviorTree'/Game/SideScrollerCPP/AI/Enermy_BehaviorTree_BP.Enermy_BehaviorTree_BP'");
	BehaviorTree = LoadObj<UBehaviorTree>(BehaviorTree_Path);
	/** Enermy Info */
	EnermyClass = FMath::RandRange(0,1);
	UE_LOG(LogTemp, Warning, TEXT("Generate EnermyClass:%d"),EnermyClass);
	/** Capsule collision */
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &AEnermyCharacter::OnOverlap);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
/** Sense */
void AEnermyCharacter::HaveSightOfPlayer(APawn* Pawn)
{
	//如果havesight,把playerlocation存入blackboard
	ApicCharacter* Player = Cast<ApicCharacter>(Pawn);
	if (Player != nullptr)
	{
		FVector Location = Player->GetActorLocation();
		AEnermyAIController* AIController = Cast<AEnermyAIController>(GetController());
		if (AIController->GetBlackboardComp()!= nullptr)
		{
			AIController->SetPlayerCaught(Location);
		}
	}
}
/** Skill */
void AEnermyCharacter::InitialThrow()
{
	FQuat QuatRotation = FQuat(FRotator(90.f, 0.f, 0.f));
	Weapon->SetActorRotation(QuatRotation);
	ThrowTime_update = ThrowTime;
	this->WeaponSpeed = 1500 * (GetCapsuleComponent()->GetForwardVector());//起始速度 
	this->WeaponLocation = GetActorLocation();
	this->WeaponA = FVector(0.f);
	EnableWeaponCollision(this);
}
void AEnermyCharacter::Throw(float deltaT)
{
	if (ThrowTime_update > 0)
	{
		ThrowTime_update -= deltaT;
		//计算新位置
		FVector NextSpeed = WeaponSpeed + WeaponA * deltaT;
		FVector NextLocation = WeaponLocation + WeaponSpeed * deltaT + 0.5f * WeaponA * deltaT * deltaT;
		//更新速度、位置、加速度
		WeaponSpeed = NextSpeed;
		WeaponLocation = NextLocation;
		FVector DeltaLocation = GetActorLocation() - WeaponLocation;
		WeaponA = (DeltaLocation - WeaponSpeed * ThrowTime_update) * 2 / (ThrowTime_update * ThrowTime_update);
		Weapon->SetActorLocation(WeaponLocation);
		Weapon->GetWeaponMesh()->SetWorldLocation(WeaponLocation);
		Weapon->AddActorLocalRotation(WeaponRotationQuat, false, 0, ETeleportType::None);//绕着武器BP本来的轴旋转
	}
	else
	{
		DisableWeaponCollision(this);
		BeginThrow = false;
	}
}
void AEnermyCharacter::FarAttack()
{
	if (!BeginThrow)//已经扔出去了就不能再扔
	{
		BeginThrow = true;
		InitialThrow();
	}
}
float AEnermyCharacter::RandomAttack()
{
	if (EnermyClass == 0)
	{
		int index = FMath::RandRange(0, SkillSet.Num() - 1);
		//int index = (ExtraLife > 0) ? (FMath::RandRange(0, BasicSkillNum - 1)) : (FMath::RandRange(BasicSkillNum, SkillSet.Num() - 1));
		UAnimMontage* RandMontage = CharacterSkill::SkillSet[index];
		float AnimTime = CastSkill(this, RandMontage, 2.0f);
		return AnimTime / 2.0f;
	}
	else
	{
		FarAttack();
		return 0.f;
	}
}
/** AI */
void AEnermyCharacter::StopBehaviorTree()
{
	AEnermyAIController* AI = Cast<AEnermyAIController>(GetController());
	if (AI)
	{
		AI->BehaviorTreeCompo->StopTree(EBTStopMode::Safe);
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor(23, 233, 4, 255), FString::Printf(TEXT("Behavor!!Stop")));
	}
}
/** Overlap */
void AEnermyCharacter::OnOverlap(class UPrimitiveComponent* HitComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Warning, TEXT("Enermy OnHiTTTTTTTTTTT!"));
}

void AEnermyCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (Sight != nullptr)
	{
		Sight->OnSeePawn.AddDynamic(this, &AEnermyCharacter::HaveSightOfPlayer);//放在tick里面会报错
	}
	/** Weapon */
	WeaponBP_Path = TEXT("Blueprint'/Game/SwordAnimsetPro/Models/Sword/WeaponActor_BP.WeaponActor_BP_C'");
	Weapon = BaseSpawnActor<AWeaponActor>(WeaponBP_Path, FVector(0.f), FRotator(0.f));
	Weapon->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	/** Enermy Property */
	UpicGameInstance* GI = Cast<UpicGameInstance>(GetWorld()->GetGameInstance());
	if (GI)
	{
		int CurrentMode = GI->GetCurrentMode();
		double DeltaTime = FPlatformTime::Seconds() - GI->GetGameBeginTime();
		float Enhance = DeltaTime * FMath::RandRange(0.9f, 1.1f) / 100.f;//每100秒的增益
		float NewHealth = MaxHealth + Enhance * 1000.f + 100.f * CurrentMode;//每100秒的增益为1000点血量
		MaxHealth = FMath::Min(NewHealth, 10000.f);
		CurrentHealth = MaxHealth;
		Weapon->SetBasicDamage(GI->GetWeaponBasicDamageSet()[CurrentMode] + Enhance * 10.f);//每100秒增益为10点基本攻击
		Weapon->SetDamageCoeff(GI->GetWeaponDamageCoeffSet()[CurrentMode] + Enhance * 0.1f);//每100秒增益为0.1伤害系数
	}
}
void AEnermyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DeathCheck(this);
	if (BeginThrow == false)
	{
		UpdateWeaponLocationRotation(this);//先关闭更新用来测试回旋镖
	}
	else
	{
		Throw(DeltaTime);
	}
}


