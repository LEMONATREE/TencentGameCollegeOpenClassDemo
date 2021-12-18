// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Blueprint/UserWidget.h"
#include "Framework/SlateDelegates.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"
#include "HealthCPP.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SceneComponent.h"
#include "TimerManager.h"
#include "picAnimInstance.h"
#include "EnermyAnimInstance.h"
#include "BaseAnimInstance.h"
#include "BaseSkill.h"
#include "WeaponActor.h"
#include "BaseCharacter.generated.h"

UCLASS()
class PIC_API ABaseCharacter : public ACharacter,public BaseSkill
{
	GENERATED_BODY()
protected:
	/** Scene */
	USceneComponent* Scene = nullptr;
	/** HealthWidget */
	FString HealthWidgetBP_Path;
	TSubclassOf<UUserWidget> HealthWidgetBP_Class;
	UHealthCPP* HealthWidget;
	UPROPERTY(EditAnywhere, Category = HealthUI)
	UWidgetComponent* HealthWidgetComp;
	void BindHealthWidget();
	void UpdateHealthWidget();
	/** Health & Energy */
	float CurrentHealth = 100.f;
	float MaxHealth = 100.f;
	float CurrentEnergy = 100.f;
	float MaxEnergy = 100.f;
	/** Revive & Death */
	int ExtraLife = 0;
	bool NowRevive = false;//正处于复活状态
	FVector RevivePoint = FVector(1200.f, -1200.f, 200.f);//Default RevivePoint
	bool Death = false;
	bool IsProcessingDeath = false;
	void OnDeath();
	/** Play Montage */
	bool IsPlayingMontage = false;
	/** Weapon */
	FString WeaponBP_Path;
	AWeaponActor* Weapon = nullptr;
	/** Player Info */
	bool IsPlayer = false;//是否为玩家
	int MaxExistEnermyNum = 0;
	int EnermyNumToSpawn = 0;
	bool ToSpawnEnermy = false;
	/** Blackhole Relevant */
	FVector TrapVelocity = FVector(0.f);//APawn中有Velocity
	FVector TrapAcceleration = FVector(0.f);	
	void ToggleBool(bool* Sig,bool BoolValue);
public:
	/** Scene */
	void FaceSceneCompToCamera();
	/** Health & Energy */
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	FORCEINLINE float GetCurrentEnergy() const { return CurrentEnergy; }
	FORCEINLINE float GetMaxEnergy() const { return MaxEnergy; }
	FORCEINLINE void SetCurrentHealth(float value) { CurrentHealth = value; }
	FORCEINLINE void SetMaxHealth(float value) { MaxHealth = value; }
	FORCEINLINE void SetCurrentEnergy(float value) { CurrentEnergy = value; }
	FORCEINLINE void SetMaxEnergy(float value) { MaxEnergy = value; }
	/** Revive & Death */
	FORCEINLINE int GetExtraLife() const { return ExtraLife; }
	FORCEINLINE bool GetNowRevive() const { return NowRevive; }
	FORCEINLINE FVector GetRevivePoint() const { return RevivePoint; }
	FORCEINLINE bool GetDeath() const { return Death; }
	FORCEINLINE bool GetIsProcessingDeath() const { return IsProcessingDeath; }
	FORCEINLINE void SetExtraLife(int value) { ExtraLife = value; }
	FORCEINLINE void SetNowRevive(bool value) {  NowRevive = value; }
	FORCEINLINE void SetRevivePoint(FVector value) { RevivePoint = value; }
	FORCEINLINE void SetDeath(bool value) { Death = value; }
	FORCEINLINE void SetIsProcessingDeath(bool value) { IsProcessingDeath = value; }
	/** Play Montage */
	FORCEINLINE bool GetIsPlayingMontage() const { return IsPlayingMontage; }
	FORCEINLINE void SetIsPlayingMontage(bool value) { IsPlayingMontage = value; }
	float BasePlayAnimMontage(UAnimMontage* MontageToPlay, UAnimInstance* AnimInstance, float PlayRate);
	/** Weapon */
	FORCEINLINE AWeaponActor* GetWeapon() const { return Weapon; }
	/** Player Info */
	FORCEINLINE bool GetIsPlayer() const { return IsPlayer; }
	FORCEINLINE int GetMaxExistEnermyNum() const { return MaxExistEnermyNum; }
	FORCEINLINE int GetEnermyNumToSpawn() const { return EnermyNumToSpawn; }
	FORCEINLINE bool GetToSpawnEnermy() const { return ToSpawnEnermy; }
	FORCEINLINE void SetIsPlayer(bool value) { IsPlayer = value; }
	FORCEINLINE void SetMaxExistEnermyNum(int value) { MaxExistEnermyNum; }
	FORCEINLINE void SetEnermyNumToSpawn(int value) { EnermyNumToSpawn = value; }
	FORCEINLINE void SetToSpawnEnermy(bool value) { ToSpawnEnermy = value; }
	/** Blackhole Relevant */
	FORCEINLINE FVector GetTrapVelocity() const { return TrapVelocity; }
	FORCEINLINE FVector GetTrapAcceleration() const { return TrapAcceleration; }
	FORCEINLINE void SetTrapVelocity(FVector value) { TrapVelocity = value; }
	FORCEINLINE void SetTrapAcceleration(FVector value) { TrapAcceleration = value; }
	/** Other Function */
	FRotator GetLookAtRotation(FVector SourceLocation, FVector TargetLocation);
	void StopTimerHandle(FTimerHandle& InHandle);
	/** Template Function */
	template<typename ObjClass>
	FORCEINLINE ObjClass* BaseSpawnActor(const FString& Path,FVector SpawnLocation,FRotator SpawnRotation)//加载蓝图资源
	{
		UClass* Load_BP = LoadBP<ObjClass>(Path);
		if (Load_BP)
		{
			FActorSpawnParameters SpawnParams;
			ObjClass* ref = GetWorld()->SpawnActor<ObjClass>(Load_BP, SpawnLocation, SpawnRotation, SpawnParams);
			return ref;
		}
		return nullptr;
	}
	template<typename ObjClass>
	FORCEINLINE void SetTimeFlow(float TimeDilation)
	{
		TSubclassOf<ObjClass> ActorToFind = ObjClass::StaticClass();
		TArray<AActor*> FoundActor;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ActorToFind, FoundActor);
		for (auto Actor : FoundActor)
		{
			Actor->CustomTimeDilation = TimeDilation;
		}
	}
protected:
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void BeginPlay() override;
public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;
};
