// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
#include "TransformToolPrivatePCH.h"
#include "TransformControl.h"
#include "ControlAxes.h"

// Sets default values
ATransformControl::ATransformControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

    CurrentStatus = EToolStatusEnum::ES_NONE;

    bOrthogonalMode = false;

    AttachedCamera = nullptr;

    OverlookActor = nullptr;

    MoveAxes = new ControlAxes();

    RotateAxes = new ControlAxes();

    ScaleAxes = new ControlAxes();

    CurrentAxes = MoveAxes;

    Center = CreateDefaultSubobject<USceneComponent> ( "center" );
    RootComponent = Center;

    InitAxes();

    SetCurrentMode ( EToolModeEnum::EM_MOVE );

}

// Called when the game starts or when spawned
void ATransformControl::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATransformControl::Tick ( float DeltaTime )
{
    Super::Tick ( DeltaTime );

    UpdateTransformControl();
}

void ATransformControl::AttachToCamera ( class UCameraComponent* Camera )
{
    AttachedCamera = Camera;
}

void ATransformControl::SetOverlookActor ( class AActor* Actor )
{
    OverlookActor = ( this == Actor ) ? OverlookActor : Actor;
}

class AActor* ATransformControl::GetOverLookActor()
{
    return OverlookActor;
}

void ATransformControl::InitAxes()
{
    InitMoveToolAxes();

    InitRotateToolAxes();

    InitScaleToolAxes();
}

void ATransformControl::InitMoveToolAxes()
{
    FColor AxisXColor ( 0xFF, 0x00, 0x00, 0xFF );
    FColor AxisYColor ( 0x00, 0xFF, 0x00, 0xFF );
    FColor AxisZColor ( 0x00, 0x00, 0xFF, 0xFF );

    UStaticMeshComponent* Center = CreateAxis ( "Center", "MoveTool", FColor ( 0xFF, 0xFF, 0xFF, 0xFF ), EToolStatusEnum::ES_AXES_CENTER );

    UStaticMeshComponent* MoveToolAxisX = CreateAxis ( "AxisX", "MoveTool", AxisXColor, EToolStatusEnum::ES_AXISX_MOVE );

    UStaticMeshComponent* MoveToolAxisY = CreateAxis ( "AxisY", "MoveTool", AxisYColor, EToolStatusEnum::ES_AXISY_MOVE );

    UStaticMeshComponent* MoveToolAxisZ = CreateAxis ( "AxisZ", "MoveTool", AxisZColor, EToolStatusEnum::ES_AXISZ_MOVE );

    UStaticMeshComponent* MoveToolAxisXY = CreateCombinationAxis ( "AxisXY", "MoveTool",
                                           AxisXColor, AxisYColor, EToolStatusEnum::ES_AXISXY_MOVE );

    UStaticMeshComponent* MoveToolAxisXZ = CreateCombinationAxis ( "AxisXZ", "MoveTool",
                                           AxisZColor, AxisXColor, EToolStatusEnum::ES_AXISXZ_MOVE );

    UStaticMeshComponent* MoveToolAxisYZ = CreateCombinationAxis ( "AxisYZ", "MoveTool",
                                           AxisYColor, AxisZColor, EToolStatusEnum::ES_AXISYZ_MOVE );

    MoveAxes->SetCenter ( Center, FColor ( 0xFF, 0xFF, 0xFF, 0xFF ) );

    MoveAxes->SetAxisX ( MoveToolAxisX, AxisXColor );
    MoveAxes->SetAxisY ( MoveToolAxisY, AxisYColor );
    MoveAxes->SetAxisZ ( MoveToolAxisZ, AxisZColor );

    MoveAxes->SetAxisXY ( MoveToolAxisXY, AxisXColor, AxisYColor );
    MoveAxes->SetAxisXZ ( MoveToolAxisXZ, AxisZColor, AxisXColor );
    MoveAxes->SetAxisYZ ( MoveToolAxisYZ, AxisYColor, AxisZColor );

    MoveAxes->HideAxes();
}

void ATransformControl::InitRotateToolAxes()
{
    FColor AxisXColor ( 0xFF, 0x00, 0x00, 0xFF );
    FColor AxisYColor ( 0x00, 0xFF, 0x00, 0xFF );
    FColor AxisZColor ( 0x00, 0x00, 0xFF, 0xFF );

    UStaticMeshComponent* RotateToolPitch = CreateAxis ( "Pitch", "RotateTool", FColor ( 0x00, 0xFF, 0x00, 0xFF ),  EToolStatusEnum::ES_PITCH );

    UStaticMeshComponent* RotateToolRoll = CreateAxis ( "Roll", "RotateTool", FColor ( 0xFF, 0x00, 0x00, 0xFF ), EToolStatusEnum::ES_ROLL );

    UStaticMeshComponent* RotateToolYaw = CreateAxis ( "Yaw", "RotateTool", FColor ( 0x00, 0x00, 0xFF, 0xFF ), EToolStatusEnum::ES_YAW );

    RotateAxes->SetAxisX ( RotateToolRoll, AxisXColor );
    RotateAxes->SetAxisY ( RotateToolPitch, AxisYColor );
    RotateAxes->SetAxisZ ( RotateToolYaw, AxisZColor );

    RotateAxes->HideAxes();
}

void ATransformControl::InitScaleToolAxes()
{
    FColor AxisXColor ( 0xFF, 0x00, 0x00, 0xFF );
    FColor AxisYColor ( 0x00, 0xFF, 0x00, 0xFF );
    FColor AxisZColor ( 0x00, 0x00, 0xFF, 0xFF );

    UStaticMeshComponent* Center = CreateAxis ( "Center", "ScaleTool", FColor ( 0xFF, 0xFF, 0xFF, 0xFF ), EToolStatusEnum::ES_AXES_CENTER );

    UStaticMeshComponent* ScaleToolAxisX = CreateAxis ( "AxisX", "ScaleTool", AxisXColor, EToolStatusEnum::ES_AXISX_SCALE );

    UStaticMeshComponent* ScaleToolAxisY = CreateAxis ( "AxisY", "ScaleTool", AxisYColor, EToolStatusEnum::ES_AXISY_SCALE );

    UStaticMeshComponent* ScaleToolAxisZ = CreateAxis ( "AxisZ", "ScaleTool", AxisZColor, EToolStatusEnum::ES_AXISZ_SCALE );

    UStaticMeshComponent* ScaleToolAxisXY = CreateCombinationAxis ( "AxisXY", "ScaleTool",
                                            AxisXColor, AxisYColor, EToolStatusEnum::ES_AXISXY_SCALE );

    UStaticMeshComponent* ScaleToolAxisXZ = CreateCombinationAxis ( "AxisXZ", "ScaleTool",
                                            AxisXColor, AxisZColor, EToolStatusEnum::ES_AXISXZ_SCALE );

    UStaticMeshComponent* ScaleToolAxisYZ = CreateCombinationAxis ( "AxisYZ", "ScaleTool",
                                            AxisYColor, AxisZColor, EToolStatusEnum::ES_AXISYZ_SCALE );

    ScaleAxes->SetCenter ( Center, FColor ( 0xFF, 0xFF, 0xFF, 0xFF ) );

    ScaleAxes->SetAxisX ( ScaleToolAxisX, AxisXColor );
    ScaleAxes->SetAxisY ( ScaleToolAxisY, AxisYColor );
    ScaleAxes->SetAxisZ ( ScaleToolAxisZ, AxisZColor );

    ScaleAxes->SetAxisXY ( ScaleToolAxisXY, AxisXColor, AxisYColor );
    ScaleAxes->SetAxisXZ ( ScaleToolAxisXZ, AxisXColor, AxisZColor );
    ScaleAxes->SetAxisYZ ( ScaleToolAxisYZ, AxisYColor, AxisZColor );

    ScaleAxes->HideAxes();
}

class UStaticMeshComponent* ATransformControl::CreateAxis ( FString Name, FString Group,
            FColor Color, EToolStatusEnum ToolStatus )
{
    UMaterial* Mat = LoadObject<UMaterial> ( NULL, TEXT ( "/Game/TransformTool/GizmoMaterial.GizmoMaterial" ) );

    UStaticMeshComponent* Axis = CreateDefaultSubobject<UStaticMeshComponent> ( * ( Group + Name ) );
    Axis->StaticMesh = LoadObject<UStaticMesh> ( NULL, *FString ( "/Game/TransformTool/" + Group + "/" + Name + "." + Name ) );
    Axis->SetCollisionProfileName ( TEXT ( "UI" ) );
    Axis->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
    Axis->bCastDynamicShadow = false;

    Axis->SetMaterial ( 0, Mat );

    UMaterialInstanceDynamic* MaterialInstanceDynamic = Axis->CreateAndSetMaterialInstanceDynamic ( 0 );
    MaterialInstanceDynamic->SetVectorParameterValue ( FName ( TEXT ( "GizmoColor" ) ), Color );

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add ( MaterialInstanceDynamic );

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add ( Color );

    Axis->OnClicked.AddDynamic ( this, &ATransformControl::OnAxisClicked );
    Axis->OnReleased.AddDynamic ( this, &ATransformControl::OnAxisReleased );
    Axis->OnBeginCursorOver.AddDynamic ( this, &ATransformControl::OnAxisBeginCursorOver );
    Axis->OnEndCursorOver.AddDynamic ( this, &ATransformControl::OnAxisEndCursorOver );

    Axis->AttachTo ( Center );

    AxisStatusMap.Add ( Axis, ToolStatus );
    MaterialColorMap.Add ( Axis, MaterialColorArray );
    MaterialInstanceMap.Add ( Axis, MaterialInstanceArray );

    return Axis;
}

class UStaticMeshComponent* ATransformControl::CreateCombinationAxis ( FString Name, FString Group,
            FColor Color1, FColor Color2, EToolStatusEnum ToolStatus )
{
    UMaterial* Mat = LoadObject<UMaterial> ( NULL, TEXT ( "/Game/TransformTool/GizmoMaterial.GizmoMaterial" ) );

    UStaticMeshComponent* CombinationAxis = CreateDefaultSubobject<UStaticMeshComponent> ( * ( Group + Name ) );
    CombinationAxis->StaticMesh = LoadObject<UStaticMesh> ( NULL, *FString ( "/Game/TransformTool/" + Group + "/" + Name + "." + Name ) );
    CombinationAxis->SetCollisionProfileName ( TEXT ( "UI" ) );
    CombinationAxis->SetCollisionEnabled ( ECollisionEnabled::QueryOnly );
    CombinationAxis->bCastDynamicShadow = false;

    CombinationAxis->SetMaterial ( 0, Mat );
    CombinationAxis->SetMaterial ( 1, Mat );

    UMaterialInstanceDynamic* MaterialInstanceDynamic1 = CombinationAxis->CreateAndSetMaterialInstanceDynamic ( 0 );
    MaterialInstanceDynamic1->SetVectorParameterValue ( FName ( TEXT ( "GizmoColor" ) ), Color1 );

    UMaterialInstanceDynamic* MaterialInstanceDynamic2 = CombinationAxis->CreateAndSetMaterialInstanceDynamic ( 1 );
    MaterialInstanceDynamic2->SetVectorParameterValue ( FName ( TEXT ( "GizmoColor" ) ), Color2 );

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add ( MaterialInstanceDynamic1 );
    MaterialInstanceArray.Add ( MaterialInstanceDynamic2 );

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add ( Color1 );
    MaterialColorArray.Add ( Color2 );

    CombinationAxis->OnClicked.AddDynamic ( this, &ATransformControl::OnAxisClicked );
    CombinationAxis->OnReleased.AddDynamic ( this, &ATransformControl::OnAxisReleased );
    CombinationAxis->OnBeginCursorOver.AddDynamic ( this, &ATransformControl::OnAxisBeginCursorOver );
    CombinationAxis->OnEndCursorOver.AddDynamic ( this, &ATransformControl::OnAxisEndCursorOver );

    CombinationAxis->AttachTo ( Center );

    AxisStatusMap.Add ( CombinationAxis, ToolStatus );
    MaterialColorMap.Add ( CombinationAxis, MaterialColorArray );
    MaterialInstanceMap.Add ( CombinationAxis, MaterialInstanceArray );

    return CombinationAxis;
}

void ATransformControl::UpdateTransformControl()
{
    if ( !IsValid( OverlookActor ) || !IsValid( AttachedCamera ) )
    {
        return;
    }

    if ( bOrthogonalMode )
    {
        OrthographicTransformControlUpdate();
    }
    else
    {
        DefaultTransformControlUpdate();
    }

    if ( EToolModeEnum::EM_SCALE == GetCurrentMode() )
    {
        SetActorRotation( OverlookActor->GetActorRotation() );
    }
    else
    {
        SetActorRotation( FRotator( 0.0f, 0.0f, 0.0f ) );
    }
}

void ATransformControl::DefaultTransformControlUpdate()
{
    FRotator ViewRotation = AttachedCamera->GetComponentRotation();
    FVector ViewLocation = AttachedCamera->GetComponentLocation();

    FVector MoveToolLocation = ( OverlookActor->GetActorLocation() - ViewLocation ).Rotation().RotateVector( FVector( 61.0f, 0.0f, 0.0f ) );
    SetActorLocation( ViewLocation + MoveToolLocation );


}

void ATransformControl::OrthographicTransformControlUpdate()
{
    float CameraOrthoWidth = AttachedCamera->OrthoWidth;

    SetActorScale3D( FVector( CameraOrthoWidth / 100.0f, CameraOrthoWidth / 100.0f, CameraOrthoWidth / 100.0f ) );

    FVector Location = GetOverLookActor()->GetActorLocation();

    SetActorLocation( FVector( Location.X, Location.Y, Location.Z + 300.0f ) );

    if ( EToolModeEnum::EM_SCALE == GetCurrentMode() )
    {
        SetActorRotation( OverlookActor->GetActorRotation() );
    }
    else
    {
        SetActorRotation( FRotator( 0.0f, 0.0f, 0.0f ) );
    }
}

void ATransformControl::SetCurrentStatus ( EToolStatusEnum Status )
{
    CurrentStatus = Status;
}

EToolStatusEnum ATransformControl::GetCurrentStatus()
{
    return CurrentStatus;
}

void ATransformControl::AutoSwitchNextMode()
{
    switch ( GetCurrentMode() )
    {
    case EToolModeEnum::EM_MOVE:
        SetCurrentMode ( EToolModeEnum::EM_ROTATE );
        break;
    case EToolModeEnum::EM_ROTATE:
        SetCurrentMode ( EToolModeEnum::EM_SCALE );
        break;
    case EToolModeEnum::EM_SCALE:
        SetCurrentMode ( EToolModeEnum::EM_MOVE );
        break;
    default:
        break;
    }
}

void ATransformControl::SetCurrentMode ( EToolModeEnum Mode )
{
    CurrentMode = Mode;

    SwitchControl ( CurrentMode );
}

EToolModeEnum ATransformControl::GetCurrentMode()
{
    return CurrentMode;
}

void ATransformControl::SwitchControl ( EToolModeEnum mode )
{
    CurrentAxes->HideAxes();

    switch ( mode )
    {
    case EToolModeEnum::EM_MOVE:
        CurrentAxes = MoveAxes;
        break;
    case EToolModeEnum::EM_ROTATE:
        CurrentAxes = RotateAxes;
        break;
    case EToolModeEnum::EM_SCALE:
        CurrentAxes = ScaleAxes;
        break;
    default:
        break;
    }

    CurrentAxes->ShowAxes();
}

void ATransformControl::SetIsOrthogonal( bool bIsOrthogonal )
{
    bOrthogonalMode = bIsOrthogonal;
}

class ControlAxes* ATransformControl::GetCurrentAxes()
{
    return CurrentAxes;
}

class ControlAxes* ATransformControl::GetMoveAxes()
{
    return MoveAxes;
}

class ControlAxes* ATransformControl::GetRotateAxes()
{
    return RotateAxes;
}

class ControlAxes* ATransformControl::GetScaleAxes()
{
    return ScaleAxes;
}

void ATransformControl::OnAxisClicked ( class UPrimitiveComponent* TouchedComponent )
{
    GEngine->AddOnScreenDebugMessage ( -1, 3.0f, FColor::Blue, "Clicked: " + TouchedComponent->GetName() );

    SetCurrentStatus ( *AxisStatusMap.Find ( Cast<UStaticMeshComponent> ( TouchedComponent ) ) );
}

void ATransformControl::OnAxisReleased ( class UPrimitiveComponent* TouchedComponent )
{
    SetCurrentStatus ( EToolStatusEnum::ES_NONE );
}

void ATransformControl::OnAxisBeginCursorOver ( class UPrimitiveComponent* TouchedComponent )
{
    SwitchMouseCursor ( EMouseCursor::Type::CardinalCross );

    if ( GetCurrentStatus() != EToolStatusEnum::ES_NONE )
    {
        return;
    }

    SetAxisCursorOverColor ( Cast<UStaticMeshComponent> ( TouchedComponent ) );
}

void ATransformControl::OnAxisEndCursorOver ( class UPrimitiveComponent* TouchedComponent )
{
    SwitchMouseCursor ( EMouseCursor::Type::Default );

    if ( GetCurrentStatus() != EToolStatusEnum::ES_NONE )
    {
        return;
    }

    RecoverAxisColor ( Cast<UStaticMeshComponent> ( TouchedComponent ) );
}

void ATransformControl::SwitchMouseCursor ( EMouseCursor::Type type )
{
    UGameplayStatics::GetPlayerController ( GetWorld(), 0 )->CurrentMouseCursor = type;
}

void ATransformControl::SetAxisCursorOverColor ( UStaticMeshComponent* Axis )
{
    GEngine->AddOnScreenDebugMessage ( -1, 3.0f, FColor::Blue, "SetAxisCursorOverColor: " + Axis->GetName() );

    CurrentAxes->SetMouseMoveIn ( Axis );
}

void ATransformControl::RecoverAxisColor ( UStaticMeshComponent* Axis )
{
    GEngine->AddOnScreenDebugMessage ( -1, 3.0f, FColor::Blue, "RecoverAxisColor" );

    CurrentAxes->SetMouseMoveOut ( Axis );
}
