// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxActor.h"
#include "DrawDebugHelpers.h"
/**Abandon*/
ATriggerBoxActor::ATriggerBoxActor()
{
	PrimaryActorTick.bCanEverTick = true;
	// Sets default values
	VisualMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	//VisualMesh->SetupAttachment(RootComponent);

	FString CubeMesh_Path = TEXT("StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'");
	UStaticMesh* CubeMesh = LoadObj<UStaticMesh>(CubeMesh_Path);
	VisualMesh->SetStaticMesh(CubeMesh);

    //Register Events
    OnActorBeginOverlap.AddDynamic(this, &ATriggerBoxActor::OnOverlapBegin);
    OnActorEndOverlap.AddDynamic(this, &ATriggerBoxActor::OnOverlapEnd);
}

// Called when the game starts or when spawned
void ATriggerBoxActor::BeginPlay()
{
	Super::BeginPlay();
	DrawDebugBox(GetWorld(), GetActorLocation(), GetComponentsBoundingBox().GetExtent(), FColor::Green, true, -1, 0, 5);

}

// Called every frame
void ATriggerBoxActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector NewLocation = GetActorLocation();
	FRotator NewRotation = GetActorRotation();
	float RunningTime = GetGameTimeSinceCreation();
	float DeltaHeight = (FMath::Sin(RunningTime + DeltaTime) - FMath::Sin(RunningTime));

	NewLocation.Z += DeltaHeight * FloatSpeed;
	float DeltaRotation = DeltaTime * RotationSpeed;

	NewRotation.Yaw += DeltaRotation;
	SetActorLocationAndRotation(NewLocation, NewRotation);


}

void ATriggerBoxActor::OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this) && OtherActor == TriggerActor)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Overlap Begin:%s"), *OtherActor->GetName()));
	}
}

void ATriggerBoxActor::OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor)
{
	//if the overlapping actor is the specific actor we identified in the editor
	if (OtherActor && (OtherActor != this) && OtherActor == TriggerActor)
	{
		GEngine->AddOnScreenDebugMessage(0, 2.f, FColor::Red, FString::Printf(TEXT("Overlap End:%s"), *OtherActor->GetName()));
	}
}