// Fill out your copyright notice in the Description page of Project Settings.


#include "Dog.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"
//用于产生血包，主要逻辑在bp中
ADog::ADog()
{
	PrimaryActorTick.bCanEverTick = true;
	MyMeshcomp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyMeshcomp"));
	MyMeshcomp->SetCollisionEnabled(ECollisionEnabled::NoCollision);   //取消物体的碰撞
	RootComponent = MyMeshcomp;	  //设置 MyMeshcomp 为根组件
}

void ADog::Dogmove(float time)
{
	FVector NowLocation = GetActorLocation();
	FVector NormalDirection = NowLocation - FVector(1100.f, -6750.f, 0.f);
	FVector TangenDirection = FVector(NormalDirection.Y, -NormalDirection.X, 0.f);
	float Move = time * Speed;
	NowLocation += TangenDirection / TangenDirection.Size() * Move;
	this->SetActorLocation(NowLocation);
}

float ADog::GetBoomTime()
{
	TSubclassOf<ADog> ApicCharacterToFind = ApicCharacter::StaticClass();
	TArray<AActor*> FoundpicCharacter;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ApicCharacterToFind, FoundpicCharacter);
	float Gap=this->BoomGap;
	for (auto Actor : FoundpicCharacter)
	{
		if (Actor->CustomTimeDilation == 0.3f)
		{
			return this->BoomGap * 20;
		}
	}
	return this->BoomGap;
}
void ADog::BulletTime()
{
	this->BoomBoomGap = (this->BoomBoomGap == 30) ? 600 : 30;
}

void ADog::SpawnUsefulActor()
{
	if (UsefulActorBP)
	{
		FActorSpawnParameters SpawnParams;
		ABome* ActorRef = GetWorld()->SpawnActor<ABome>(UsefulActorBP, GetTransform(), SpawnParams);
	}
}
void ADog::BeginPlay()
{
	Super::BeginPlay();
}
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Dogmove(DeltaTime);
	if (this->CustomTimeDilation == 0.05f)
	{
		this->BoomBoomGap = 600;
	}
	else if(this->BoomBoomGap==600)
	{
		this->BoomBoomGap = 30;//重置
		this->Now %= 30;
	}
	this->Now++;
	if (this->Now > this->BoomBoomGap)
	{
		this->Now %= this->BoomBoomGap;
		this->SpawnUsefulActor();
	}
}

