// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"

#include "ControlAxes.h"

const int32 ID_AXIS_CENTER = 0;
const int32 ID_AXIS_X = 1;
const int32 ID_AXIS_Y = 2;
const int32 ID_AXIS_Z = 3;
const int32 ID_AXIS_XY = 4;
const int32 ID_AXIS_XZ = 5;
const int32 ID_AXIS_YZ = 6;

const int32 NUMBER_OF_AXES = 7;

ControlAxes::ControlAxes()
{
    AxesIDs.SetNum( NUMBER_OF_AXES );
    Center = nullptr;
    AxisX = nullptr;
    AxisY = nullptr;
    AxisZ = nullptr;
    AxisXY = nullptr;
    AxisXZ = nullptr;
    AxisYZ = nullptr;
}

ControlAxes::~ControlAxes()
{
}

class UStaticMeshComponent* ControlAxes::GetCenter()
{
    return Center;
}

class UStaticMeshComponent* ControlAxes::GetAxisX()
{
    return AxisX;
}

class UStaticMeshComponent* ControlAxes::GetAxisY()
{
    return AxisY;
}

class UStaticMeshComponent* ControlAxes::GetAxisZ()
{
    return AxisZ;
}

class UStaticMeshComponent* ControlAxes::GetAxisXY()
{
    return AxisXY;
}

class UStaticMeshComponent* ControlAxes::GetAxisXZ()
{
    return AxisXZ;
}

class UStaticMeshComponent* ControlAxes::GetAxisYZ()
{
    return AxisYZ;
}

void ControlAxes::SetCenter( class UStaticMeshComponent* Axis, FColor Color )
{
    Center = Axis;
    CenterColor = Color;
    CenterMaterialInstance = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxesIDs[ID_AXIS_CENTER] = Axis;
}

void ControlAxes::SetAxisX( class UStaticMeshComponent* Axis, FColor Color )
{
    AxisX = Axis;
    AxisXColor = Color;
    AxisXMaterialInstance = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxesIDs[ID_AXIS_X] = Axis;
}

void ControlAxes::SetAxisY( class UStaticMeshComponent* Axis, FColor Color )
{
    AxisY = Axis;
    AxisYColor = Color;
    AxisYMaterialInstance = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxesIDs[ID_AXIS_Y] = Axis;
}

void ControlAxes::SetAxisZ( class UStaticMeshComponent* Axis, FColor Color )
{
    AxisZ = Axis;
    AxisZColor = Color;
    AxisZMaterialInstance = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxesIDs[ID_AXIS_Z] = Axis;
}

void ControlAxes::SetAxisXY( class UStaticMeshComponent* Axis, FColor Color1, FColor Color2 )
{
    AxisXY = Axis;

    AxisXYColor1 = Color1;
    AxisXYColor2 = Color2;

    AxisXYMaterialInstance1 = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxisXYMaterialInstance2 = Axis->CreateAndSetMaterialInstanceDynamic( 1 );

    AxesIDs[ID_AXIS_XY] = Axis;
}

void ControlAxes::SetAxisXZ( class UStaticMeshComponent* Axis, FColor Color1, FColor Color2 )
{
    AxisXZ = Axis;

    AxisXZColor1 = Color1;
    AxisXZColor2 = Color2;

    AxisXZMaterialInstance1 = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxisXZMaterialInstance2 = Axis->CreateAndSetMaterialInstanceDynamic( 1 );

    AxesIDs[ID_AXIS_XZ] = Axis;
}

void ControlAxes::SetAxisYZ( class UStaticMeshComponent* Axis, FColor Color1, FColor Color2 )
{
    AxisYZ = Axis;
    AxisYZColor1 = Color1;
    AxisYZColor2 = Color2;

    AxisYZMaterialInstance1 = Axis->CreateAndSetMaterialInstanceDynamic( 0 );
    AxisYZMaterialInstance2 = Axis->CreateAndSetMaterialInstanceDynamic( 1 );

    AxesIDs[ID_AXIS_YZ] = Axis;
}

void ControlAxes::SetMouseMoveIn( class UStaticMeshComponent* Axis )
{
    FColor MouseMoveInColor( 0xFF, 0xFF, 0x00, 0xFF );

    int32 index = 0;

    for ( index = 0; index < AxesIDs.Num(); ++index )
    {
        if ( Axis != AxesIDs[index] )
        {
            continue;
        }

        break;
    }

    switch ( index )
    {
    case ID_AXIS_X:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_Y:
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_Z:
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_XY:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisXYMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisXYMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_XZ:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisXZMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisXZMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_YZ:
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisYZMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisYZMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    case ID_AXIS_CENTER:
        CenterMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), MouseMoveInColor );
        break;
    default:
        break;
    }
}

void ControlAxes::SetMouseMoveOut( class UStaticMeshComponent* Axis )
{
    int32 index = 0;

    for ( index = 0; index < AxesIDs.Num(); ++index )
    {
        if ( Axis != AxesIDs[index] )
        {
            continue;
        }

        break;
    }

    switch ( index )
    {
    case ID_AXIS_X:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXColor );
        break;
    case ID_AXIS_Y:
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYColor );
        break;
    case ID_AXIS_Z:
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisZColor );
        break;
    case ID_AXIS_XY:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXColor );
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYColor );
        AxisXYMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXYColor1 );
        AxisXYMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXYColor2 );
        break;
    case ID_AXIS_XZ:
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisZColor );
        AxisXZMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXZColor1 );
        AxisXZMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXZColor2 );
        break;
    case  ID_AXIS_YZ:
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisZColor );
        AxisYZMaterialInstance1->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYZColor1 );
        AxisYZMaterialInstance2->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYZColor2 );
        break;
    case ID_AXIS_CENTER:
        CenterMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), CenterColor );
        AxisXMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisXColor );
        AxisYMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisYColor );
        AxisZMaterialInstance->SetVectorParameterValue( FName( TEXT( "GizmoColor" ) ), AxisZColor );
        break;
    default:
        break;
    }
}

void ControlAxes::ShowAxes()
{

    SetAxisEnable( AxisX, true );
    SetAxisEnable( AxisY, true );
    SetAxisEnable( AxisZ, true );

    if ( !AxisXY )
    {
        return;
    }
    SetAxisEnable( Center, true );

    SetAxisEnable( AxisXY, true );
    SetAxisEnable( AxisXZ, true );
    SetAxisEnable( AxisYZ, true );
}

void ControlAxes::HideAxes()
{
    SetAxisEnable( AxisX, false );
    SetAxisEnable( AxisY, false );
    SetAxisEnable( AxisZ, false );

    if ( !AxisXY )
    {
        return;
    }

    SetAxisEnable( Center, false );

    SetAxisEnable( AxisXY, false );
    SetAxisEnable( AxisXZ, false );
    SetAxisEnable( AxisYZ, false );
}

void ControlAxes::SetAxisEnable( class UStaticMeshComponent* Axis, bool bEnable )
{
    bEnable ? Axis->SetCollisionEnabled( ECollisionEnabled::QueryOnly ) : Axis->SetCollisionEnabled( ECollisionEnabled::NoCollision );
    Axis->SetVisibility( bEnable );
}
