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

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Center, meta = (AllowPrivateAccess = "true"))
    class USphereComponent * m_pCenter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * m_pAxisX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * m_pAxisY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UArrowComponent * m_pAxisZ;

    void initAxis();

    class UArrowComponent* createAxis(FString name);

};
