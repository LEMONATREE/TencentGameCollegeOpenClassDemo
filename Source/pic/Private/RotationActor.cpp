// Fill out your copyright notice in the Description page of Project Settings.


#include "RotationActor.h"
#include "F:/Unreal Project/pic/Source/pic/picCharacter.h"

ARotationActor::ARotationActor()
{ 	
	PrimaryActorTick.bCanEverTick = true;
	/** Mesh */
	FString CubeMesh_Path = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	UStaticMesh* CubeMesh = LoadObj<UStaticMesh>(CubeMesh_Path);
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetStaticMesh(CubeMesh);
}

/** Collision & Overlap */
void ARotationActor::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Rotation Overlap Begin1:%s"), *OtherActor->GetName()));
	if (OtherActor && (OtherActor != this))
	{
		//TODO:切换视角到blackhole
		ApicCharacter* Player = Cast<ApicCharacter>(OtherActor);
		if (Player && IntoBossGame == false)
		{
			TSubclassOf<ARotationActor> ARotationActorToFind = ARotationActor::StaticClass();
			TArray<AActor*> FoundActor;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARotationActorToFind, FoundActor);
			FVector TargetLocation = FVector(-1400.f, -300.f, 0.f);
			float DelayTime = 0.f;
			for (auto Actor : FoundActor)
			{
				//控制已经存在于map中的rotationactor移动
				ARotationActor* tmp = Cast<ARotationActor>(Actor);
				if (tmp && tmp->IntoBossGame == true)
				{
					float LoopTime = FMath::RandRange(0.005f, 0.01f);
					DelayTime += FMath::RandRange(1.f, 2.f);//如果DelayTime为0会出问题
					//UE_LOG(LogTemp, Warning, TEXT("%f,%f,%f,%f"), TargetLocation.X,TargetLocation.Y,TargetLocation.Z,DelayTime);
					FTimerDelegate EndDel = FTimerDelegate::CreateUObject(tmp, &ARotationActor::DelayMove, tmp, TargetLocation, LoopTime);
					tmp->GetWorld()->GetTimerManager().SetTimer(tmp->EndHandle, EndDel, DelayTime, false);//延时一段时间后执行
					TargetLocation += FVector(-1000.f, 0.f, 50.f);
				}
			}
			//SetTimer只能调用本地函数，不能在这延时调用player的函数?
			//因此只能在pic的spawn函数中设置一个delaytime参数以控制其生成延时。
			DelaySpawnBlackHole(2.f);//自己生成
			DelaySetViewOnBlackHole();
			IntoBossGame = true;//防止运行两次、防止运行自身
		}
	}
}
void ARotationActor::SetViewOnBlackHole()
{
	ApicCharacter* Player = Cast<ApicCharacter>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (Player && MyBlackHole)
	{
		StopTimerHandle(ViewHandle);
		Player->SetViewOnBlackHole(MyBlackHole);
	}
}
void ARotationActor::DelaySetViewOnBlackHole()
{
	//每隔0.1s尝试执行，直到执行成功
	GetWorld()->GetTimerManager().SetTimer(ViewHandle, this, &ARotationActor::SetViewOnBlackHole, 0.1f, true);
}
/** Move */
void ARotationActor::MoveTo(FVector TargetLocation, float DeltaTime)
{
	FVector CurrentLocation = GetActorLocation();
	FVector DeltaLocation = TargetLocation - CurrentLocation;
	if (DeltaLocation.Size() < 10.f && MoveDown == true)
	{
		StopTimerHandle(BeginHandle);
		return;
	}
	FVector Speed = DeltaLocation / DeltaLocation.Size() * 400;
	FVector NewLocation = CurrentLocation + Speed * DeltaTime;
	SetActorLocation(NewLocation);
}
void ARotationActor::ThreeStepMove(FVector TargetLocation,float DeltaTime)
{
	//先控制block的xy坐标对上，再控制高度>200,最后到达target
	FVector CurrentLocation = GetActorLocation();
	//FVector DeltaLocation = TargetLocation - CurrentLocation;
	FVector TmpTargetLocation = FVector(TargetLocation);
	TmpTargetLocation.Z = TargetLocation.Z + 200.f;
	FVector TmpDeltaLocation = TmpTargetLocation - CurrentLocation;
	if (TmpDeltaLocation.Size() < 10.f)
	{
		MoveDown = true;
	}
	if (MoveDown == false)
	{
		if (abs(TmpDeltaLocation.X) > 10.f || abs(TmpDeltaLocation.Y) > 10.f)//如果还没到同一xy，就不设z方向速度
		{
			TmpTargetLocation.Z = CurrentLocation.Z;
			MoveTo(TmpTargetLocation, DeltaTime);
		}
		else if (TmpDeltaLocation.Z > 0.f)//到了同一xy,但是还没上升到一定高度
		{
			MoveTo(TmpTargetLocation, DeltaTime);
		}
	}
	else
	{
		MoveTo(TargetLocation, DeltaTime);
	}
}
void ARotationActor::DelayMove(ARotationActor* tmp, FVector TargetLocation, float LoopTime)
{
	FTimerDelegate MoveDel = FTimerDelegate::CreateUObject(tmp, &ARotationActor::ThreeStepMove, TargetLocation, LoopTime);
	tmp->GetWorld()->GetTimerManager().SetTimer(tmp->BeginHandle, MoveDel, LoopTime, true);
}
/** Timer */
void ARotationActor::StopTimerHandle(FTimerHandle &InHandle)//输入handle必须使用handle引用
{
	UE_LOG(LogTemp, Warning, TEXT("Activate StopHandle"));
	GetWorld()->GetTimerManager().ClearTimer(InHandle);//可以override到base里
}
/** Spawn Blackhole */
void ARotationActor::SpawnBlackHole()
{
	FString Path = TEXT("Blueprint'/Game/MyResource/MyBlackHoleActor.MyBlackHoleActor_C'");
	FVector SpawnLocation = FVector(-3500.f, -300.f, 400.f);
	FRotator SpawnRotation = FRotator(0.f, 180.f, 0.f);
	UClass* Load_BP = LoadBP<ABlackHoleActor>(Path);//没有读取蓝图会崩溃
	FTransform SpawnTransform(SpawnRotation, SpawnLocation);
	MyBlackHole = Cast<ABlackHoleActor>(UGameplayStatics::BeginDeferredActorSpawnFromClass(this, Load_BP, SpawnTransform));
	if (MyBlackHole)//延迟spawn的初始化过程
	{
		MyBlackHole->SetIntoBossGame(false);
		MyBlackHole->SetNewScale3D(FVector(40.f));
		MyBlackHole->SetScaleSpeed(2.f);
	}
	UGameplayStatics::FinishSpawningActor(MyBlackHole, SpawnTransform);//延迟spawn
}
void ARotationActor::DelaySpawnBlackHole(float DelayTime)
{
	if (DelayTime <= 0.f)
	{
		SpawnBlackHole();
	}
	else
	{
		//返回类型不为void时不能用settimer
		FTimerHandle SpawnHandle;
		GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &ARotationActor::SpawnBlackHole, DelayTime);//延时一段时间后执行	
	}
}

void ARotationActor::BeginPlay()
{
	Super::BeginPlay();	
	FScriptDelegate Del;
	Del.BindUFunction(this, TEXT("OnOverlapBegin"));
	OnActorBeginOverlap.Add(Del);
}
void ARotationActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));
	NewLocation.Z += DeltaHeight * FloatSpeed;          //按FloatSpeed调整高度
	float DeltaRotation = DeltaTime * RotationSpeed;    //每秒旋转等于RotationSpeed的角度
	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);
}

