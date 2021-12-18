// Copyright Epic Games, Inc. All Rights Reserved.

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
#include "BaseCharacter.h"
#include "EnermyCharacter.h"
#include "Components/BoxComponent.h"
#include "WeaponActor.h"
#include "CharacterSkill.h"
#include "BaseCharacterInterface.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "BlackHoleActor.h"
#include "ComboWidget.h"
#include "picCharacter.generated.h"

UCLASS(config=Game)
class ApicCharacter : public ABaseCharacter, public CharacterSkill,public IBaseCharacterInterface
{
	GENERATED_BODY()
private:
	/** Side view camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mySpringArm1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mySpringArm2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* mySpringArm3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraBoom1;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraBoom2;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraBoom3;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildComp;
protected:
	/** GetView */
	int NowCamera = 0;
	TArray<UCameraComponent*> MyCamera;
	void GetFirstPersonView();
	void GetThirdPersonView();
	bool IsThirdPersonView();
	/** Move */
	void MoveRight(float Val);
	void MoveForward(float Val);
	FVector GetFirstPersonDirection(FString DirectName);
	FVector GetThirdPersonDirection(FString DirectName);
	/** Jump */
	void JumpStarted(const ETouchIndex::Type FingerIndex, const FVector Location);
	void JumpStopped(const ETouchIndex::Type FingerIndex, const FVector Location);
	/** SpeedUp */
	bool IsSpeedUp = false;
	float BaseSpeed = 600.f;
	FTimerHandle SpeedUpHandle;
	void SpeedUp();
	void SpeedDown();
	void UpdateEnergy();
	/** SpawnEnermy */
	FString EnermyBP_Path;
	void SpawnEnermy();
	void DelaySpawnEnermy(float DelayTime);
	/** Combo */
	FString ComboWidgetBP_Path;	
	TSubclassOf<UUserWidget> ComboWidgetBP_Class;
	UPROPERTY()
	class UWidgetComponent* ComboWidgetComp;
	UPROPERTY()
	class UComboWidget* ComboWidget;
	int ComboCount = 0;
	FTimerHandle ComboUpdateHandle;
	FTimerHandle ComboResetHandle;
	void BindComboWidget();
	void UpdateComboWidget();
	void ResetComboWidget();
	/** Skill */
	void BulletTime();
public:
	/** GetView */
	FORCEINLINE UCameraComponent* GetCameraBoom1() const { return CameraBoom1; }
	FORCEINLINE UCameraComponent* GetCameraBoom2() const { return CameraBoom2; }
	FORCEINLINE UCameraComponent* GetCameraBoom3() const { return CameraBoom3; }
	FORCEINLINE USpringArmComponent* GetSpringArm1() const { return mySpringArm1; }
	FORCEINLINE USpringArmComponent* GetSpringArm2() const { return mySpringArm2; }
	FORCEINLINE USpringArmComponent* GetSpringArm3() const { return mySpringArm3; }
	/** SpeedUp */
	FORCEINLINE float GetBaseSpeed() const { return BaseSpeed; }
	void SetSpeedParam(bool SpeedUpState, float MoveSpeed);//设置与SpeedUp/SpeedDown有关的参数
	/** Combo */
	FORCEINLINE int GetComboCount() const { return ComboCount; }
	FORCEINLINE void UpdateComboCount() { ComboCount++; }
	FORCEINLINE FTimerHandle& GetComboResetHandle() { return ComboResetHandle; }
	void UpdateComboWidgetWithTime();
	void DelayResetComboWidget();
	/** Skill*/
	void Skill_W();
	void Skill_E();
	void Skill_R();

protected:
	/** Input */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
	virtual void BeginPlay() override;
public:
	ApicCharacter();
	virtual void Tick(float DeltaTime) override;

	void SetViewOnBlackHole(ABlackHoleActor* BlackHole);
	void DelaySetViewOnBlackHole(ABlackHoleActor* BlackHole, float DelayTime);
	void ResetViewFromBlackHole();
	void DelayResetViewFromBlackHole(float DelayTime);

};
