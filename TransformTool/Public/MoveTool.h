// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
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

    void BindingTool( class UCameraComponent* Camera);

    void SetOverlookActor( class AActor* Actor);

    void UpdateMoveToolPosition();

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent * BoxX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent * BoxY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UBoxComponent * BoxZ;

    class UCameraComponent* AttachedCamera;

    class AActor* OverlookActor;

    void InitAxis();

    void InitBox();

    void InitCombinationAxis();

    class UArrowComponent* CreateAxis(FString name);

    class UBoxComponent* CreateBox(FString name);

    class UStaticMeshComponent* CreateCombinationAxis(FString name);

    UFUNCTION()
    void OnAxisXClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisXReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZReleased(class UPrimitiveComponent* TouchedComponent);
};
