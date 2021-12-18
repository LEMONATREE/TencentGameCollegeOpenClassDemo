// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Bome.h"
#include "Dog.h"
#include "picAnimInstance.h"
#include "BlackHoleActor.h"
#include "picGameInstance.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	/** Scene */
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	Scene->SetupAttachment(GetRootComponent());
	Scene->SetRelativeLocation(FVector(0.f, 0.f, 10.f));
	/** HealthWidget */
	HealthWidgetBP_Path = TEXT("WidgetBlueprint'/Game/SideScrollerCPP/UI/Health.Health_C'");
	HealthWidgetBP_Class = LoadBP<UUserWidget>(HealthWidgetBP_Path);
	HealthWidgetComp = CreateDefaultSubobject<UWidgetComponent>(TEXT("Health"));
	HealthWidgetComp->AttachToComponent(Scene, FAttachmentTransformRules::KeepRelativeTransform);
	HealthWidgetComp->SetCollisionProfileName(TEXT("NoCollision"));//Avoid unnecessray collision
	HealthWidgetComp->SetRelativeLocation(FVector(0.f, 0.f, 130.f));
	HealthWidgetComp->SetWidgetClass(HealthWidgetBP_Class);
}
/** Scene */
void ABaseCharacter::FaceSceneCompToCamera()
{
	FVector SceneCompLocation = Scene->GetComponentLocation();
	FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();
	FRotator Turn = GetLookAtRotation(SceneCompLocation, CameraLocation);
	Scene->SetWorldRotation(Turn);
}
/** HealthWidget */
void ABaseCharacter::BindHealthWidget()
{
	if (HealthWidgetBP_Class != nullptr)
	{
		HealthWidget = CreateWidget<UHealthCPP>(GetWorld()->GetGameInstance(), HealthWidgetBP_Class);
		if (HealthWidget && HealthWidgetComp)
		{
			HealthWidgetComp->SetWidget(HealthWidget);
		}
	}
}
void ABaseCharacter::UpdateHealthWidget()
{
	if (HealthWidget && HealthWidgetComp)
	{
		HealthWidget->UpdateHealth(CurrentHealth, MaxHealth, CurrentEnergy, MaxEnergy);
	}
}
/** Death& Revive */
void ABaseCharacter::OnDeath()
{
	Death = true;
	if (ExtraLife > 0)
	{
		ExtraLife--;
		GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("Remaining lives:%d"), ExtraLife));
		NowRevive = true;
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("You are dead!")));
		//1.受到攻击同时会有多段伤害，前面的伤害导致actor触发Nowrevive=true，但是要过一会才会revive，
		//后面的伤害继续触发Nowrevive和life--，所以立刻destroy导致错误
		//2.这里直接destroy会无法播放死亡动画，需要在tick里面逐帧判断
		//在tick里面要判断是否处于death或者revive状态，同时要在destroy前一刻才能修改
		//否则当enermy只剩最后一条命时，通过DeathCheck置0的death在下一帧又触发ondeath被修改为1
	}
}
/** Play Montage */
float ABaseCharacter::BasePlayAnimMontage(UAnimMontage* MontageToPlay, UAnimInstance* AnimInstance, float PlayRate)//看看是否能替代castskill
{
	if (AnimInstance != nullptr)
	{
		float AnimTime = AnimInstance->Montage_Play(MontageToPlay, PlayRate);
		return AnimTime;
	}
	return 0.0f;
}
/** Other Function */
FRotator ABaseCharacter::GetLookAtRotation(FVector SourceLocation, FVector TargetLocation)
{
	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(SourceLocation, TargetLocation);
	return FRotator(0, Turn.Yaw, 0);//只旋转z轴
}
void ABaseCharacter::StopTimerHandle(FTimerHandle& InHandle)//输入必须使用handle引用
{
	GetWorld()->GetTimerManager().ClearTimer(InHandle);
}

float ABaseCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	UE_LOG(LogTemp, Warning, TEXT("Damage:%f!!!"), Damage);
	if (Damage < 0.f || Damage > 10000000.f)//忽略错误的伤害
	{
		return 0.f;
	}
	if (!NowRevive)//不处于复活状态
	{
		CurrentHealth = FMath::Max(CurrentHealth - Damage, 0.f);
		if (CurrentHealth == 0.f) // 受到足够的伤害了,进入死亡处理
		{
			OnDeath();
		}
		else//受伤
		{
			if (IsPlayingMontage == false)//Enermy播放受伤动画 
			{
				IsPlayingMontage = true;
				//面向伤害来源 Target->Source
				FVector SourceLocation = DamageCauser->GetActorLocation();
				FVector TargetLocation = GetActorLocation();
				FRotator Turn = GetLookAtRotation(TargetLocation, SourceLocation);
				SetActorRotation(Turn);
				//播放受击动画
				auto* AnimInstance = GetMesh()->GetAnimInstance();
				float AnimTime = BasePlayAnimMontage(BeAttacked_Montage, AnimInstance, 1.f);
				//播放结束后重置IsPlayingMontage
				FTimerHandle OutHandle;
				FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ABaseCharacter::SetIsPlayingMontage, false);//用来传递参数，会根据函数的声明来给定可输入参数数量
				GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, AnimTime / 1.f, false);
			}
		}
	}
	return 0.0f;
}
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	BindHealthWidget();
}
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateHealthWidget();
	FaceSceneCompToCamera();
}

