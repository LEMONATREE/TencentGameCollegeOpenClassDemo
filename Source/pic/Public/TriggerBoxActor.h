// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
//#include "RotationActor.h"
#include "LoadResource.h"
#include "TriggerBoxActor.generated.h"
/**Abandon*/
UCLASS()
class PIC_API ATriggerBoxActor : public ATriggerBox, public LoadResource
{
	GENERATED_BODY()
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:
	// constructor sets default values for this actor's properties
	ATriggerBoxActor();
	// overlap begin function
	UFUNCTION()
	void OnOverlapBegin(class AActor* OverlappedActor, class AActor* OtherActor);
	// overlap end function
	UFUNCTION()
	void OnOverlapEnd(class AActor* OverlappedActor, class AActor* OtherActor);

	// specific actor for overlap
	UPROPERTY(EditAnywhere)
	AActor* TriggerActor;

	//AActor* MeshActor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
	float FloatSpeed = 20.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FloatingActor")
	float RotationSpeed = 20.0f;
	// Sets default values for this actor's properties
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* VisualMesh;

	virtual void Tick(float DeltaTime) override;
};
