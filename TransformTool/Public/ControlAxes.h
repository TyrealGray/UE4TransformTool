// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Engine.h"
/**
 *
 */
class ControlAxes
{
public:
    ControlAxes();
    ~ControlAxes();

    void SetCenter(class UStaticMeshComponent* Axis, FColor Color);

    void SetAxisX(class UStaticMeshComponent* Axis, FColor Color);

    void SetAxisY(class UStaticMeshComponent* Axis, FColor Color);

    void SetAxisZ(class UStaticMeshComponent* Axis, FColor Color);

    void SetAxisXY(class UStaticMeshComponent* Axis, FColor Color1,  FColor Color2);

    void SetAxisXZ(class UStaticMeshComponent* Axis, FColor Color1, FColor Color2);

    void SetAxisYZ(class UStaticMeshComponent* Axis, FColor Color1, FColor Color2);

    class UStaticMeshComponent* GetCenter();

    class UStaticMeshComponent* GetAxisX();

    class UStaticMeshComponent* GetAxisY();

    class UStaticMeshComponent* GetAxisZ();

    class UStaticMeshComponent* GetAxisXY();

    class UStaticMeshComponent* GetAxisXZ();

    class UStaticMeshComponent* GetAxisYZ();

    void SetMouseMoveIn(class UStaticMeshComponent* Axis);

    void SetMouseMoveOut(class UStaticMeshComponent* Axis);

    void ShowAxes();

    void HideAxes();

private:
    FColor CenterColor;

    FColor AxisXColor;
    FColor AxisYColor;
    FColor AxisZColor;

    FColor AxisXYColor1;
    FColor AxisXYColor2;

    FColor AxisXZColor1;
    FColor AxisXZColor2;

    FColor AxisYZColor1;
    FColor AxisYZColor2;

    class UStaticMeshComponent* Center;
    class UStaticMeshComponent* AxisX;
    class UStaticMeshComponent* AxisY;
    class UStaticMeshComponent* AxisZ;
    class UStaticMeshComponent* AxisXY;
    class UStaticMeshComponent* AxisXZ;
    class UStaticMeshComponent* AxisYZ;

    class UMaterialInstanceDynamic* CenterMaterialInstance;

    class UMaterialInstanceDynamic* AxisXMaterialInstance;
    class UMaterialInstanceDynamic* AxisYMaterialInstance;
    class UMaterialInstanceDynamic* AxisZMaterialInstance;

    class UMaterialInstanceDynamic* AxisXYMaterialInstance1;
    class UMaterialInstanceDynamic* AxisXYMaterialInstance2;

    class UMaterialInstanceDynamic* AxisXZMaterialInstance1;
    class UMaterialInstanceDynamic* AxisXZMaterialInstance2;

    class UMaterialInstanceDynamic* AxisYZMaterialInstance1;
    class UMaterialInstanceDynamic* AxisYZMaterialInstance2;

    TArray<UStaticMeshComponent*> AxesIDs;

    void SetAxisEnable(class UStaticMeshComponent* Axis, bool bEnable);

};
