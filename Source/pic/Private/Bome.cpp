// Fill out your copyright notice in the Description page of Project Settings.


#include "Bome.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
#include "Dog.h"

ABome::ABome()
{
 	PrimaryActorTick.bCanEverTick = true;
	/** Mesh */
	MyMeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshcomp"));
	RootComponent = MyMeshcomp;	  //设置 MyMeshcomp 为根组件
	/** Collision & Overlap */
	MySphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("MySphereComp"));
	MySphereComp->SetupAttachment(MyMeshcomp);
	MySphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	/** SetTimeFlow */
	TSubclassOf<ADog> ApicCharacterToFind = ADog::StaticClass();
	TArray<AActor*> FoundpicCharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ApicCharacterToFind, FoundpicCharacter);
	for (auto Actor : FoundpicCharacter)
	{
		if (Actor->CustomTimeDilation == 0.05f)
		{
			this->CustomTimeDilation = 0.05f;//修改时间流速而不是速度
		}
		break;
	}
}
/** Drop */
void ABome::BomeDrop(float time)
{
	FVector NowLocation = GetActorLocation();
	float speed = this->DropSpeed;
	float Move = time * speed;
	NowLocation.Z -= Move;
	this->SetActorLocation(NowLocation);
}
/** Destroy */
void ABome::BomeDestroy()
{
	Destroy();
}
void ABome::BomeDelayDestroy()//延时销毁
{
	FVector NowLocation = GetActorLocation();
	if (NowLocation.Z < (this->DestroyZ)&& !SetDestroy)
	{
		SetDestroy = true;
		DropSpeed = 0.f;
		FTimerHandle OnHandle;
		GetWorld()->GetTimerManager().SetTimer(OnHandle, this, &ABome::BomeDestroy, 500.0f, false);
	}
}
/** Collision & Overlap */
void ABome::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	if (OtherActor && (OtherActor != this))
	{
		ApicCharacter* Player = Cast<ApicCharacter>(OtherActor);
		if (Player)
		{
			GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Cure")));
			float NewHealth = FMath::Min(Player->GetCurrentHealth() + Player->GetMaxHealth() * 0.1f + 100.f, Player->GetMaxHealth());
			Player->SetCurrentHealth(NewHealth);
			Destroy();
		}
	}
}

void ABome::BeginPlay()
{
	Super::BeginPlay();
	FScriptDelegate Del;
	Del.BindUFunction(this, TEXT("OnOverlapBegin"));
	OnActorBeginOverlap.Add(Del);
}
void ABome::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	AddActorWorldRotation(FRotator(0.f,2.f,0.f));
	BomeDrop(DeltaTime);
	BomeDelayDestroy();
}

