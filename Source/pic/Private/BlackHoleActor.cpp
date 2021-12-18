// Fill out your copyright notice in the Description page of Project Settings.


#include "BlackHoleActor.h"
#include "BaseCharacter.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "EnermyCharacter.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
#include "picGameInstance.h"
#include "TimerManager.h"

// Sets default values
ABlackHoleActor::ABlackHoleActor()
{
 	PrimaryActorTick.bCanEverTick = true;
	/** Mesh */
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshcomp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/** Collision & Overlap */
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("MySphereComp"));
	SphereComp->SetupAttachment(MeshComp);
	SphereComp->SetSphereRadius(1000);
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ABlackHoleActor::OverLap);
}
/** Collision & Overlap */
void ABlackHoleActor::OverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApicCharacter* Player = Cast<ApicCharacter>(OtherActor);
	if (Player)
	{
		if (IntoBossGame == false)//加载boss关
		{
			if (OtherActor)
			{
				OtherActor->CustomTimeDilation = 0.2;
			}
			IntoBossGame = true;
			SphereComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);//12.15
			GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &ABlackHoleActor::IntoBoss, 1.f);	
		}
		else if(!IsDamageApplied)//boss技能伤害
		{
			IsDamageApplied = true;
			if (BlackHoleClass == 0)//Fire
			{
				float damage = FMath::RandRange(400.f, 600.f) + Player->GetMaxHealth() * FMath::RandRange(0.02f, 0.08f);
				UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, UDamageType::StaticClass());
			}
			else//Thunder
			{
				float damage = FMath::RandRange(100.f, 150.f);
				UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, UDamageType::StaticClass());
				float NewEnergy = FMath::Max(Player->GetCurrentEnergy() - FMath::RandRange(200.f, 300.f), 0.f);
				Player->SetCurrentEnergy(NewEnergy);
				Player->SetSpeedParam(false, Player->GetBaseSpeed());
			}
		}
	}
}
/** IntoBossMap */
void ABlackHoleActor::AttractEnermy(float DeltaTime)
{
	if (DuringTime > 0.f)
	{
		DuringTime -= DeltaTime;
		TSubclassOf<ABaseCharacter> ABaseCharacterToFind = ABaseCharacter::StaticClass();
		TArray<AActor*> FoundBaseCharacter;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ABaseCharacterToFind, FoundBaseCharacter);
		for (auto FoundActor : FoundBaseCharacter)
		{
			ABaseCharacter* Actor = Cast<ABaseCharacter>(FoundActor);
			if (Actor->GetIsPlayer())//////////
				continue;
			FVector ActorLocation = Actor->GetActorLocation();
			FVector BlackHoleLocation = this->GetActorLocation();
			FVector DeltaLocation = BlackHoleLocation - ActorLocation;
			//计算新位置
			FVector V = Actor->GetTrapVelocity();
			FVector A = Actor->GetTrapAcceleration();
			FVector NextV = V + A * DeltaTime;
			FVector NextA = (DeltaLocation - V * DuringTime) * 2 / (DuringTime * DuringTime);
			FVector NextLocation = ActorLocation + V * DeltaTime + 0.5f * A * DeltaTime * DeltaTime;
			//更新速度、位置、加速度
			Actor->SetTrapVelocity(NextV);
			Actor->SetTrapAcceleration(NextA);
			//显示
			Actor->SetActorLocation(NextLocation);
		}
	}
}
void ABlackHoleActor::IntoBoss()
{
	UWorld* World = GetWorld();
	if (World)
	{
		UpicGameInstance* GI = Cast<UpicGameInstance>(World->GetGameInstance());
		if (GI)
		{
			GI->SetCurrentMap(2);//Boss关
		}
		UE_LOG(LogTemp, Warning, TEXT("into Boss"))
		FString NewMap = FString::Printf(TEXT("/Game/SideScrollerCPP/Maps/BossMap_1"));
		World->ServerTravel(NewMap);
	}
}
/** BossSkill */
void ABlackHoleActor::CastBlackHole(float DeltaTime)
{
	FVector NewLocation = GetActorLocation() + Speed * DeltaTime;
	SetActorLocation(NewLocation);
	NewScale3D += FVector(ScaleSpeed);
	AddActorWorldOffset(FVector(0.f, 0.f, -40 * ScaleSpeed));//25
	SetActorScale3D(NewScale3D);
}
/** Scale */
void ABlackHoleActor::InitScale(float a,float b)
{
	IntoBossGame = false;
	NewScale3D = FVector(a);
	ScaleSpeed = b;
}
void ABlackHoleActor::ChangeScale(float DeltaTime)
{
	NewScale3D += FVector(ScaleSpeed);
	FVector NewLocation = GetActorLocation() + Speed * DeltaTime;
	SetActorLocation(NewLocation);
	AddActorWorldOffset(FVector(0.f, 0.f, -40 * ScaleSpeed));//25
	SetActorScale3D(NewScale3D);
	if (NewScale3D.Size() > 100.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(OutHandle);
	}
}
/** Destroy */
void ABlackHoleActor::BlackHoleDestroy()
{
	this->Destroy();
}

void ABlackHoleActor::BeginPlay()
{
	Super::BeginPlay();
	if (IntoBossGame == false)
	{
		NewScale3D = FVector(4.f);
		ScaleSpeed = 0.2f;
	}
	else
	{
		NewScale3D = FVector(1.f);
		ScaleSpeed = 0.04f;
	}
	SetActorScale3D(NewScale3D);
	float TickTime = 0.1f;
	FTimerDelegate Del = FTimerDelegate::CreateUObject(this, &ABlackHoleActor::ChangeScale, TickTime);
	GetWorld()->GetTimerManager().SetTimer(OutHandle, Del, TickTime, true);
	if (IntoBossGame == true)//在boss关
	{
		GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &ABlackHoleActor::BlackHoleDestroy, 10.f);//最多存在10s
	}
}
void ABlackHoleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IntoBossGame == false)
	{
		AttractEnermy(DeltaTime);
	}
	else
	{
		CastBlackHole(DeltaTime);
	}
}

