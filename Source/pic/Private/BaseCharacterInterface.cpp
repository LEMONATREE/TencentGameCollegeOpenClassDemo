// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseCharacterInterface.h"
#include "RotationActor.h"
#include "TimerManager.h"
#include "picGameInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EnermyCharacter.h"
/** Weapon */
void IBaseCharacterInterface::UpdateWeaponLocationRotation(ABaseCharacter* Character)
{
	if (Character->GetWeapon())
	{
		FVector Location;
		FQuat Rotation;
		Character->GetMesh()->GetSocketWorldLocationAndRotation(TEXT("weapon"), Location, Rotation);
		Character->GetWeapon()->SetActorLocation(Location);
		Character->GetWeapon()->SetActorRotation(Rotation);
		Character->GetWeapon()->GetWeaponMesh()->SetWorldLocationAndRotation(Location, Rotation);
	}
}
void IBaseCharacterInterface::EnableWeaponCollision(ABaseCharacter* Character)
{
	if (Character->GetWeapon())
	{
		Character->GetWeapon()->SetActorEnableCollision(ECollisionEnabled::QueryOnly);//可以改成有物理碰撞
	}
}
void IBaseCharacterInterface::DisableWeaponCollision(ABaseCharacter* Character)
{
	if (Character->GetWeapon())
	{
		Character->GetWeapon()->SetActorEnableCollision(ECollisionEnabled::NoCollision);
	}
}
/** Skill */
float IBaseCharacterInterface::CastSkill(ABaseCharacter* Character, UAnimMontage* Skill_key, float PlayRate)
{
	auto* AnimInstance = Character->GetMesh()->GetAnimInstance();
	//如果没有在播放死亡动画或者处于死亡状态，则可以释放技能
	if (!Character->GetDeath() && !Character->GetIsPlayingMontage() && !AnimInstance->Montage_IsPlaying(Character->Death_Montage))
	{
		StartCastSkill(Character);
		float AnimTime = Character->BasePlayAnimMontage(Skill_key, AnimInstance, PlayRate);
		FTimerHandle OutHandle;
		FTimerDelegate Del = FTimerDelegate::CreateRaw(this, &IBaseCharacterInterface::FinishCastSkill, Character);
		Character->GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, AnimTime / PlayRate, false);//隔一段时间结束释放技能
		return AnimTime;
	}
	return 0.0f;
}
void IBaseCharacterInterface::StartCastSkill(ABaseCharacter* Character)
{
	Character->SetIsPlayingMontage(true);
	EnableWeaponCollision(Character);
}
void IBaseCharacterInterface::FinishCastSkill(ABaseCharacter* Character)
{
	DisableWeaponCollision(Character);
	Character->SetIsPlayingMontage(false);
}
/** Death & Revive */
void IBaseCharacterInterface::DeathCheck(ABaseCharacter* Character)
{
	if (Character->GetDeath() && !Character->GetIsProcessingDeath())
	{
		auto* AnimInstance = Character->GetMesh()->GetAnimInstance();
		if (AnimInstance != nullptr)
		{
			Character->SetIsProcessingDeath(true);
			Character->SetIsPlayingMontage(false);//最高优先级 打断其他动画
			float tt = Character->BasePlayAnimMontage(Character->Death_Montage, AnimInstance, 1.0f);
			FTimerHandle DeathHandle;
			if (Character->GetNowRevive())
			{
				FTimerDelegate Del = FTimerDelegate::CreateRaw(this, &IBaseCharacterInterface::Revive, Character);
				Character->GetWorld()->GetTimerManager().SetTimer(DeathHandle, Del, tt*0.8f, false);
			}
			else
			{
				UpicGameInstance* GI = Cast<UpicGameInstance>(Character->GetWorld()->GetGameInstance());
				if (GI)
				{
					if (!Character->GetIsPlayer())//敌人死亡
					{
						//关闭BehaviorTree
						AEnermyCharacter* AI = Cast<AEnermyCharacter>(Character);
						AI->StopBehaviorTree();
						//更新PlayerInfo
						float NewScore = GI->GetPlayerScore() + (Character->GetMaxHealth()) * (GI->GetCurrentMode() + 1.f);
						GI->SetPlayerScore(NewScore);
						GI->PlayerLevelUp();
						//Check是否进入boss关
						if (GI->GetPlayerScore() > 200.f && !GI->GetRotationActorIsSpawn())
						{
							//生成进入boss关的机关
							FString RotationActorBP_Path = TEXT("Blueprint'/Game/MyResource/MyRotationActor.MyRotationActor_C'");
							ARotationActor* ref = Character->BaseSpawnActor<ARotationActor>(RotationActorBP_Path, FVector(1300.f, 200.f, 320.f), FRotator(0.f));
							ref->SetIntoBossGame(false);
							ref->SetRotationSpeed(20.f);
							GI->SetRotationActorIsSpawn(true);
							//删除阻挡的staticmesh
							DestroyActorFromName<AStaticMeshActor>(Character->GetWorld(), FString("Wall12"));
						}
						//准备接下来的敌人
						ABaseCharacter* Player = Cast<ABaseCharacter>(Character->GetWorld()->GetFirstPlayerController()->GetCharacter());
						if (Player)
						{
							UpdateLevel(Player);
							if (GI->GetTotalEnermyNum() > 0)
							{
								Player->SetEnermyNumToSpawn(Player->GetEnermyNumToSpawn() + 1);
								Player->SetToSpawnEnermy((Player->GetEnermyNumToSpawn() == Player->GetMaxExistEnermyNum()));
								GI->SetTotalEnermyNum(GI->GetTotalEnermyNum() - 1);
							}
							else
							{
								Player->SetToSpawnEnermy((Player->GetEnermyNumToSpawn() != 0));
							}
						}
					}
					else//玩家死亡
					{
						//记录当前积分
						TSubclassOf<ASaveGameActor> SaveGameActorToFind = ASaveGameActor::StaticClass();
						TArray<AActor*> FoundActor;
						UGameplayStatics::GetAllActorsOfClass(Character->GetWorld(), SaveGameActorToFind, FoundActor);
						if (FoundActor.Num() != 0)
						{
							auto SaveGameActor = Cast<ASaveGameActor>(FoundActor[0]);
							SaveGameActor->SaveGameFile();
							GEngine->AddOnScreenDebugMessage(0, 2.f, FColor(23, 233, 4, 255), FString::Printf(TEXT("记录击杀分数:%.0f"), GI->GetPlayerScore()));
						}
						//删除所有Enermy/Boss
						TSubclassOf<AEnermyCharacter> EnermyToFind = AEnermyCharacter::StaticClass();
						TArray<AActor*> FoundEnermy;
						UGameplayStatics::GetAllActorsOfClass(Character->GetWorld(), EnermyToFind, FoundEnermy);
						for (auto Enermy : FoundEnermy)
						{
							Enermy->Destroy();
						}
					}
				}
				FTimerDelegate Del = FTimerDelegate::CreateRaw(this, &IBaseCharacterInterface::DestroyCharacter, Character);//用来传递参数，会根据函数的声明来给定可输入参数数量
				Character->GetWorld()->GetTimerManager().SetTimer(DeathHandle, Del, tt*0.8f, false);
			}
		}
	}
}
void IBaseCharacterInterface::Revive(ABaseCharacter* Character)
{
	Character->SetCurrentHealth(Character->GetMaxHealth());
	Character->SetActorLocation(Character->GetRevivePoint());//根据怪物和玩家区分重生位置
	FVector now = Character->GetActorLocation();
	//UE_LOG(LogTemp, Warning, TEXT("You're Reborn at (%f,%f,%f)!"), now.X, now.Y, now.Z);
	Character->SetIsPlayingMontage(false);//重置
	Character->SetNowRevive(false);//复活完毕
	Character->SetDeath(false);
	Character->SetIsProcessingDeath(false);
}
void IBaseCharacterInterface::DestroyCharacter(ABaseCharacter* Character)//提供接口给子类使用
{
	Character->GetWeapon()->Destroy();
	Character->Destroy();
}
/** Player Info */
void IBaseCharacterInterface::UpdateLevel(ABaseCharacter* Character)
{
	UpicGameInstance* GI = Cast<UpicGameInstance>(Character->GetWorld()->GetGameInstance());
	if (GI)
	{
		Character->SetMaxHealth(100000.f * FMath::Exp2(GI->GetPlayerLevel()));
		Character->SetCurrentHealth(Character->GetMaxHealth());
		Character->SetCurrentEnergy(Character->GetMaxEnergy());
		auto* Weapon = Character->GetWeapon();
		if (Weapon)
		{
			//GEngine->AddOnScreenDebugMessage(0, 10.f, FColor::Red, FString::Printf(TEXT("uplevel")));
			Weapon->SetBasicDamage(Weapon->GetBasicDamage() + 50.f); 
		}
	}
}