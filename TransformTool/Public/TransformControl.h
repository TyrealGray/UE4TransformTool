// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "TransformControl.generated.h"

UENUM(BlueprintType)
enum class EToolModeEnum : uint8
{
    EM_MOVE UMETA(DisplayName = "Move"),
    EM_ROTATE 	UMETA(DisplayName = "Rotate"),
    EM_SCALE	UMETA(DisplayName = "Scale")
};


UENUM(BlueprintType)
enum class EToolStatusEnum : uint8
{
    ES_NONE UMETA(DisplayName = "None"),

    ES_AXES_CENTER UMETA(DisplayName = "AxesCenter"),

    ES_AXISX_MOVE UMETA(DisplayName = "AxisXMove"),
    ES_AXISY_MOVE UMETA(DisplayName = "AxisYMove"),
    ES_AXISZ_MOVE UMETA(DisplayName = "AxisZMove"),
    ES_AXISXY_MOVE UMETA(DisplayName = "AxisXYMove"),
    ES_AXISXZ_MOVE	 UMETA(DisplayName = "AxisXZMove"),
    ES_AXISYZ_MOVE	 UMETA(DisplayName = "AxisYZMove"),

    ES_PITCH UMETA(DisplayName = "Pitch"),
    ES_ROLL UMETA(DisplayName = "Roll"),
    ES_YAW UMETA(DisplayName = "Yaw"),

    ES_AXISX_SCALE UMETA(DisplayName = "AxisXScale"),
    ES_AXISY_SCALE UMETA(DisplayName = "AxisYScale"),
    ES_AXISZ_SCALE UMETA(DisplayName = "AxisZScale"),
    ES_AXISXY_SCALE UMETA(DisplayName = "AxisXYScale"),
    ES_AXISXZ_SCALE UMETA(DisplayName = "AxisXZScale"),
    ES_AXISYZ_SCALE UMETA(DisplayName = "AxisYZScale")
};


UCLASS()
class TRANSFORMTOOL_API ATransformControl : public AActor
{
    GENERATED_BODY()

public:
    // Sets default values for this actor's properties
    ATransformControl();

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    // Called every frame
    virtual void Tick( float DeltaSeconds ) override;

    void AttachToCamera( class UCameraComponent* Camera);

    void SetOverlookActor( class AActor* Actor);

    class AActor* GetOverLookActor();

    void SetCurrentStatus(EToolStatusEnum Status);

    EToolStatusEnum GetCurrentStatus();

    void SetCurrentMode(EToolModeEnum Mode);

    EToolModeEnum GetCurrentMode();

    void AutoSwitchNextMode();

private:

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, meta = (AllowPrivateAccess = "true"))
    EToolStatusEnum CurrentStatus;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = "true"))
    EToolModeEnum CurrentMode;

    class USceneComponent * Center;

    class UStaticMeshComponent * ControlCenter;

    class UCameraComponent* AttachedCamera;

    class AActor* OverlookActor;

    TArray<UStaticMeshComponent*> MoveToolAxes;

    TArray<UStaticMeshComponent*> RotateToolAxes;

    TArray<UStaticMeshComponent*> ScaleToolAxes;

    TMap<FString, EToolStatusEnum> AxisStatusMap;

    TMap<FString, TArray<FColor>> MaterialColorMap;

    TMap<FString, TArray<UMaterialInstanceDynamic*>> MaterialInstanceMap;

    void InitAxes();

    void InitMoveToolAxes();

    void InitRotateToolAxes();

    void InitScaleToolAxes();

    class UStaticMeshComponent* CreateAxis(FString Name, FString Group,
                                               FColor Color, EToolStatusEnum ToolStatus);

    class UStaticMeshComponent* CreateCombinationAxis(FString Name, FString Group,
                FColor Color1, FColor Color2, EToolStatusEnum ToolStatus);

    void UpdateTransformControlPosition();

    void SwitchMouseCursor(EMouseCursor::Type type);

    void SwitchControl(EToolModeEnum mode);

    void SetAxesMeshComponentEnable(const TArray<UStaticMeshComponent*> &AxesMesh, bool bEnable);

    void SetAxisMeshComponentEnable(class UStaticMeshComponent* MeshComponent, bool bEnable);

    void HideAllMeshComponents();

    void SetAxisCursorOverColor(FString AxisName);

    void RecoverAxisColor(FString AxisName);

    UFUNCTION()
    void OnAxisClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisBeginCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent);

};
