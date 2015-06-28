// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "MoveTool.generated.h"

UENUM(BlueprintType)
enum class EMoveToolStatusEnum : uint8
{
    ES_NONE UMETA(DisplayName = "None"),
    ES_AXISX 	UMETA(DisplayName = "AxisX"),
    ES_AXISY	UMETA(DisplayName = "AxisY"),
    ES_AXISZ	UMETA(DisplayName = "AxisZ"),
    ES_AXISXY	UMETA(DisplayName = "AxisXY"),
    ES_AXISXZ	UMETA(DisplayName = "AxisXZ"),
    ES_AXISYZ	UMETA(DisplayName = "AxisYZ")
};


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

    class AActor* GetOverLookActor();

    EMoveToolStatusEnum GetCurrentStatus();

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, meta = (AllowPrivateAccess = "true"))
    EMoveToolStatusEnum CurrentStatus;

    class UCameraComponent* AttachedCamera;

    class AActor* OverlookActor;

    void InitAxis();

    void InitBox();

    void InitCombinationAxis();

    class UArrowComponent* CreateAxis(FString Name, FColor Color, FRotator Rotation);

    class UBoxComponent* CreateBox(FString Name, FVector Location, FVector Scale3D);

    class UStaticMeshComponent* CreateCombinationAxis(FString Name, FVector Location, FVector Scale3D);

    UFUNCTION()
    void OnAxisXClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisXReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisXBeginCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisXEndCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYBeginCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisYEndCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZBeginCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisZEndCursorOver(class UPrimitiveComponent* TouchedComponent);

    void SetCurrentStatus(EMoveToolStatusEnum Status);

    void SwitchMouseCursor(EMouseCursor::Type type);
};
