// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponActor.h"
#include "BaseCharacter.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
#include "BaseCharacterInterface.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = true;
	/** Mesh */
	WeaponMesh_Path = TEXT("StaticMesh'/Game/SwordAnimsetPro/Models/Sword/Sword.Sword'");
	WeaponMesh = LoadObj<UStaticMesh>(WeaponMesh_Path);
	WeaponMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Weapon"));
	WeaponMeshComp->SetStaticMesh(WeaponMesh);
	/** Collision & Overlap */
	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Collision"));
	WeaponCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
void AWeaponActor::OnOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	AWeaponActor* NowWeapon = Cast<AWeaponActor>(OverlappedActor);
	ABaseCharacter* NowCharacter = Cast<ABaseCharacter>(OtherActor);
	if (NowWeapon && NowCharacter)
	{
		if (NowWeapon->IsPlayer != NowCharacter->GetIsPlayer())
		{
			if (!NowCharacter->GetIsPlayer())//Enermy受到攻击->朝向受击方向，更新combo
			{
				ApicCharacter* Player = Cast<ApicCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
				if (Player)
				{
					if (Player->GetComboCount() == 0)//开始更新combo,隔一段时间reset
					{
						Player->UpdateComboWidgetWithTime();
					}
					else//重置延时
					{
						Player->StopTimerHandle(Player->GetComboResetHandle());
					}
					Player->UpdateComboCount();//更新combo
					Player->DelayResetComboWidget();//延时清除combo		
				}
			}
			else//Player受到攻击->朝向受击方向，相应逻辑在BaseCharacter中
			{ 
			}
			float damage = (BasicDamage + FMath::RandRange(0.0f, 5.0f)) * DamageCoeff;
			UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, UDamageType::StaticClass()); // 造成伤害
		}
	}
}
void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	if (WeaponCollisionComp != nullptr)
	{
		FScriptDelegate Del;
		Del.BindUFunction(this, TEXT("OnOverlap"));
		OnActorBeginOverlap.Add(Del);
	}
}


