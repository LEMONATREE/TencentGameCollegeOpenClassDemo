// Fill out your copyright notice in the Description page of Project Settings.


#include "RingActor.h"
#include "BaseCharacter.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
#include "TimerManager.h"

ARingActor::ARingActor()
{
	PrimaryActorTick.bCanEverTick = true;
	/** Mesh */
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshcomp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	/** Collision & Overlap */
	int Num = 18;
	for (int i = 0; i < Num; i++)
	{
		FString Name = TEXT("Collision")+ FString::FromInt(i+1);
		UBoxComponent* tmp= CreateDefaultSubobject<UBoxComponent>(FName(*Name));
		tmp->SetupAttachment(MeshComp);
		tmp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		float len = 65.f;
		float angle = 2.f * 3.1415926f / Num * i;//弧度值
		float deltaX = len * FMath::Sin(angle);
		float deltaY = len * FMath::Cos(angle);
		tmp->SetRelativeLocation(FVector(deltaX, deltaY, 0.f));
		tmp->InitBoxExtent(FVector(32.f, 32.f, 5.f));
		tmp->OnComponentBeginOverlap.AddDynamic(this, &ARingActor::OverLap);
		MyCollisionSet.Add(tmp);
	}
}
/** Collision & Overlap */
void ARingActor::OverLap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ApicCharacter* Player = Cast<ApicCharacter>(OtherActor);
	if (Player && !IsDamageApplied)
	{
		IsDamageApplied = true;
		float damage = FMath::RandRange(100.f, 1000.f);
		UGameplayStatics::ApplyDamage(OtherActor, damage, nullptr, this, UDamageType::StaticClass());
		if (Player->GetCurrentEnergy() > 50.f)
		{
			Player->SetCurrentEnergy(Player->GetCurrentEnergy() - 50.f);
		}
		else
		{
			Player->SetCurrentEnergy(0.f);
		}
		Player->SetSpeedParam(false, Player->GetBaseSpeed());
	}
}
/** Destroy */
void ARingActor::RingDestroy()
{
	this->Destroy();
}

void ARingActor::BeginPlay()
{
	Super::BeginPlay();
	SetActorScale3D(NewScale3D);
	FTimerHandle OutHandle;
	GetWorld()->GetTimerManager().SetTimer(OutHandle, this, &ARingActor::RingDestroy, 6.f);//最多存在6s
}
void ARingActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	NewScale3D += FVector(0.15f) + NewScale3D * 0.1;
	SetActorScale3D(NewScale3D);	
}

