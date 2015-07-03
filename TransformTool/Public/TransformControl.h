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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Center, meta = (AllowPrivateAccess = "true"))
    class USceneComponent * Center;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ControlCenter;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status, meta = (AllowPrivateAccess = "true"))
    EToolStatusEnum CurrentStatus;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mode, meta = (AllowPrivateAccess = "true"))
    EToolModeEnum CurrentMode;

    #pragma region MoveToolProperty

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisX;
    class UMaterialInstanceDynamic * MoveToolAxisXMatIns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisY;
    class UMaterialInstanceDynamic * MoveToolAxisYMatIns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisZ;
    class UMaterialInstanceDynamic * MoveToolAxisZMatIns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisXY;
    class UMaterialInstanceDynamic * MoveToolAxisXYMatIns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisXZ;
    class UMaterialInstanceDynamic * MoveToolAxisXZMatIns;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * MoveToolAxisYZ;
    class UMaterialInstanceDynamic * MoveToolAxisYZMatIns;

    #pragma endregion

    #pragma region RotateToolProperty

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * RotateToolPitch;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * RotateToolRoll;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * RotateToolYaw;

    #pragma endregion

    #pragma region ScaleToolProperty

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisX;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisXZ;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisXY;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Axis, meta = (AllowPrivateAccess = "true"))
    class UStaticMeshComponent * ScaleToolAxisYZ;

    #pragma endregion

    class UCameraComponent* AttachedCamera;

    class AActor* OverlookActor;

    TMap<FString, TArray<FColor>> MaterialColorMap;

    TMap<FString, TArray<UMaterialInstanceDynamic*>> MaterialInstanceMap;

    void InitCenter();

    void InitAxis();

    void InitCombinationAxis();

    class UStaticMeshComponent* CreateAxis(FString Name, FString Group, FColor Color);

    class UStaticMeshComponent* CreateCombinationAxis(FString Name, FString Group, FColor Color1, FColor Color2);

    void UpdateTransformControlPosition();

    void SwitchMouseCursor(EMouseCursor::Type type);

    void SwitchControl(EToolModeEnum mode);

    void HideAllMeshComponents();

    void SetAxisMeshComponentEnable(class UStaticMeshComponent* MeshComponent, bool bEnable);

    void SetAxisCursorOverColor(FString AxisName);

    void RecoverAxisColor(FString AxisName);

    #pragma region MoveToolEvent

    UFUNCTION()
    void OnMoveTooXClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnMoveToolYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnMoveToolZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnMoveToolXYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnMoveToolXZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnMoveToolYZClicked(class UPrimitiveComponent* TouchedComponent);

    #pragma endregion

    #pragma region RotateToolEvent

    UFUNCTION()
    void OnRotateToolPitchClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnRotateToolRollClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnRotateToolYawClicked(class UPrimitiveComponent* TouchedComponent);

    #pragma endregion

    #pragma region ScaleToolEvent

    UFUNCTION()
    void OnScaleToolXClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnScaleToolYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnScaleToolZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnScaleToolXYClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnScaleToolXZClicked(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnScaleToolYZClicked(class UPrimitiveComponent* TouchedComponent);

    #pragma endregion

    UFUNCTION()
    void OnAxisReleased(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisBeginCursorOver(class UPrimitiveComponent* TouchedComponent);

    UFUNCTION()
    void OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent);

};
