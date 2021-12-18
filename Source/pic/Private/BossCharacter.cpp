// Fill out your copyright notice in the Description page of Project Settings.


#include "BossCharacter.h"
#include "BlackHoleActor.h"
#include "TimerManager.h"
ABossCharacter::ABossCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	EnermyClass = 2;//Boss
	/** Resource */
	FString StabAttack = TEXT("/Game/SwordAnimsetPro/UE4_Mannequin/Mesh/BossStab_Montage");
	StabAttack_Montage = LoadMontage(StabAttack);
	RingBP_Path = TEXT("Blueprint'/Game/MyResource/MyRingActor.MyRingActor_C'");
	FireBallBP_Path = TEXT("Blueprint'/Game/MyResource/MyFireBallActor.MyFireBallActor_C'");
	ThunderBallBP_Path = TEXT("Blueprint'/Game/MyResource/MyThunderBallActor.MyThunderBallActor_C'");
}
void ABossCharacter::BossSkill()
{
	UE_LOG(LogTemp, Warning, TEXT("Short-Distance Attack"));
	IsAttack = true;
	FVector BossLocation = GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();
	//pitch,yaw,roll => y,z,x
	Ring = BaseSpawnActor<ARingActor>(RingBP_Path, BossLocation, FRotator(0.f));
	Ring->SetActorScale3D(FVector(1.f));
	if (IsStab == true)
	{
		Ring->SetIsStabAttack(true);
	}
	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(BossLocation, PlayerLocation);
	if (RingClass == 0)
	{
		Turn = FRotator(Turn.Pitch, Turn.Yaw, Turn.Roll + 90.f);
		RingClass = 1;
	}
	else
	{
		RingClass = 0;
	}
	Ring->SetActorRotation(Turn.Quaternion());
	FTimerHandle OutHandle;
	FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ABossCharacter::SetIsAttack, false);
	GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, 1.f, false);
}
void ABossCharacter::BossSkill2()
{
	UE_LOG(LogTemp, Warning, TEXT("Long-Distance Attack"));
	IsAttack = true;
	FVector BossLocation = GetActorLocation();
	FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetCharacter()->GetActorLocation();
	int BlackHoleClass = FMath::RandRange(0, 1);
	ABlackHoleActor* myBlackHole;
	if (BlackHoleClass == 0)
	{
		myBlackHole = BaseSpawnActor<ABlackHoleActor>(FireBallBP_Path, BossLocation, FRotator(0.f));
		myBlackHole->SetBlackHoleClass(0);
	}
	else
	{
		myBlackHole = BaseSpawnActor<ABlackHoleActor>(ThunderBallBP_Path, BossLocation, FRotator(0.f));
		myBlackHole->SetBlackHoleClass(1);
	}
	myBlackHole->SetActorScale3D(FVector(1.f));
	FRotator Turn = UKismetMathLibrary::FindLookAtRotation(BossLocation, PlayerLocation);
	FVector Direction = UKismetMathLibrary::GetForwardVector(Turn);
	FVector NewSpeed = Direction / Direction.Size() * FMath::RandRange(500.f, 1000.f);
	myBlackHole->SetSpeed(NewSpeed);
	myBlackHole->SetIntoBossGame(true);	
	FTimerHandle OutHandle;
	FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ABossCharacter::SetIsAttack, false);
	GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, 1.f, false);
}
void ABossCharacter::HideWeapon()
{
	if (Weapon)
	{
		Weapon->SetActorHiddenInGame(true);
		//Weapon->Destroy();//Boss不需要武器
		//Weapon = nullptr;
	}
}
void ABossCharacter::DelayHideWeapon()
{
	FTimerHandle OutHandle;
	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &ABossCharacter::HideWeapon, 1.f);
}
void ABossCharacter::BeginPlay()
{
	Super::BeginPlay();
	HideWeapon();
	Weapon->SetActorScale3D(FVector(5.f));
	MaxHealth = 10000.f;
	CurrentHealth = 10000.f;
}
void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

