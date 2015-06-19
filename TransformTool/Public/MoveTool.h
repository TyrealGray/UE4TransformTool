// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "TransformToolPrivatePCH.h"
#include "GameFramework/Actor.h"
#include "MoveTool.generated.h"

UCLASS()
class TRANSFORMTOOL_API AMoveTool : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    AMoveTool();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    UFUNCTION()
    void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Center, meta = (AllowPrivateAccess = "true"))
    class USphereComponent * Center;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * AxisX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * AxisY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * AxisZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * AxisXZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * AxisXY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * AxisYZ;

    void InitAxis();

    void InitCombinationAxis();

    class UArrowComponent* CreateAxis(FString name);

    class UStaticMeshComponent* CreateCombinationAxis(FString name);

};
