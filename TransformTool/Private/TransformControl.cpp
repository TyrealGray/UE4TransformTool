// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
#include "TransformToolPrivatePCH.h"
#include "TransformControl.h"

// Sets default values
ATransformControl::ATransformControl()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

    CurrentStatus = EToolStatusEnum::ES_NONE;

    AttachedCamera = nullptr;

    OverlookActor = nullptr;

    Center = CreateDefaultSubobject<USceneComponent>("center");
    RootComponent = Center;

    InitCenter();

    InitAxis();

    InitCombinationAxis();

    SetCurrentMode(EToolModeEnum::EM_MOVE);
}

// Called when the game starts or when spawned
void ATransformControl::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void ATransformControl::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );

    UpdateTransformControlPosition();
}

void ATransformControl::AttachToCamera(class UCameraComponent* Camera)
{
    AttachedCamera = Camera;
}

void ATransformControl::SetOverlookActor(class AActor* Actor)
{
    OverlookActor = (this == Actor) ? OverlookActor : Actor;
}

class AActor* ATransformControl::GetOverLookActor()
{
    return OverlookActor;
}

void ATransformControl::InitCenter()
{
    ControlCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MoveToolCenter"));
    ControlCenter->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/MoveTool/MoveToolCenter.MoveToolCenter"));
    ControlCenter->AttachTo(Center);
}

void ATransformControl::InitAxis()
{
    MoveToolAxisX = CreateAxis("AxisX", "MoveTool", FColor(0xFF, 0x00, 0x00, 0xFF));
    MoveToolAxisX->OnClicked.AddDynamic(this, &ATransformControl::OnMoveTooXClicked);

    MoveToolAxisY = CreateAxis("AxisY", "MoveTool", FColor(0x00, 0xFF, 0x00, 0xFF));
    MoveToolAxisY->OnClicked.AddDynamic(this, &ATransformControl::OnMoveToolYClicked);

    MoveToolAxisZ = CreateAxis("AxisZ", "MoveTool", FColor(0x00, 0x00, 0xFF, 0xFF));
    MoveToolAxisZ->OnClicked.AddDynamic(this, &ATransformControl::OnMoveToolZClicked);

    RotateToolPitch = CreateAxis("Pitch", "RotateTool", FColor(0xFF, 0x00, 0x00, 0xFF));
    RotateToolPitch->OnClicked.AddDynamic(this, &ATransformControl::OnRotateToolPitchClicked);

    RotateToolRoll = CreateAxis("Roll", "RotateTool", FColor(0x00, 0xFF, 0x00, 0xFF));
    RotateToolRoll->OnClicked.AddDynamic(this, &ATransformControl::OnRotateToolRollClicked);

    RotateToolYaw = CreateAxis("Yaw", "RotateTool", FColor(0x00, 0x00, 0xFF, 0xFF));
    RotateToolYaw->OnClicked.AddDynamic(this, &ATransformControl::OnRotateToolYawClicked);

    ScaleToolAxisX = CreateAxis("AxisX", "ScaleTool", FColor(0xFF, 0x00, 0x00, 0xFF));
    ScaleToolAxisX->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolXClicked);

    ScaleToolAxisY = CreateAxis("AxisY", "ScaleTool", FColor(0x00, 0xFF, 0x00, 0xFF));
    ScaleToolAxisY->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolYClicked);

    ScaleToolAxisZ = CreateAxis("AxisZ", "ScaleTool", FColor(0x00, 0x00, 0xFF, 0xFF));
    ScaleToolAxisZ->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolZClicked);
}

class UStaticMeshComponent* ATransformControl::CreateAxis(FString Name, FString Group, FColor Color)
{
    UStaticMeshComponent* Axis = CreateDefaultSubobject<UStaticMeshComponent>(*(Group + Name));

    Axis->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/" + Group + "/" + Name + "." + Name));

    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));

    Axis->SetMaterial(0, Mat);
    UMaterialInstanceDynamic* MaterialInstanceDynamic = Axis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color);
    Axis->SetCollisionProfileName(FName("UI"));

    Axis->OnReleased.AddDynamic(this, &ATransformControl::OnAxisReleased);
    Axis->OnBeginCursorOver.AddDynamic(this, &ATransformControl::OnAxisBeginCursorOver);
    Axis->OnEndCursorOver.AddDynamic(this, &ATransformControl::OnAxisEndCursorOver);

    Axis->AttachTo(Center);

    return Axis;
}

void ATransformControl::InitCombinationAxis()
{
    MoveToolAxisXY = CreateCombinationAxis("AxisXY", "MoveTool", FColor(0x00, 0xFF, 0x00, 0xFF), FColor(0xFF, 0x00, 0x00, 0xFF));
    MoveToolAxisXY->OnClicked.AddDynamic(this, &ATransformControl::OnMoveToolXYClicked);

    MoveToolAxisXZ = CreateCombinationAxis("AxisXZ", "MoveTool", FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0xFF, 0x00, 0x00, 0xFF));
    MoveToolAxisXZ->OnClicked.AddDynamic(this, &ATransformControl::OnMoveToolXZClicked);

    MoveToolAxisYZ = CreateCombinationAxis("AxisYZ", "MoveTool", FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0x00, 0xFF, 0x00, 0xFF));
    MoveToolAxisYZ->OnClicked.AddDynamic(this, &ATransformControl::OnMoveToolYZClicked);

    ScaleToolAxisXY = CreateCombinationAxis("AxisXY", "ScaleTool", FColor(0xFF, 0x00, 0x00, 0xFF), FColor(0x00, 0xFF, 0x00, 0xFF));
    ScaleToolAxisXY->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolXYClicked);

    ScaleToolAxisXZ = CreateCombinationAxis("AxisXZ", "ScaleTool", FColor(0xFF, 0x00, 0x00, 0xFF), FColor(0x00, 0x00, 0xFF, 0xFF));
    ScaleToolAxisXZ->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolXZClicked);

    ScaleToolAxisYZ = CreateCombinationAxis("AxisYZ", "ScaleTool", FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0x00, 0xFF, 0x00, 0xFF));
    ScaleToolAxisYZ->OnClicked.AddDynamic(this, &ATransformControl::OnScaleToolYZClicked);
}

class UStaticMeshComponent* ATransformControl::CreateCombinationAxis(FString Name, FString Group, FColor Color1, FColor Color2)
{
    UStaticMeshComponent* CombinationAxis = CreateDefaultSubobject<UStaticMeshComponent>(*(Group + Name));

    CombinationAxis->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/" + Group + "/" + Name + "." + Name));
    CombinationAxis->SetCollisionProfileName(FName("UI"));

    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));
    CombinationAxis->SetMaterial(0, Mat);
    CombinationAxis->SetMaterial(1, Mat);

    UMaterialInstanceDynamic* MaterialInstanceDynamic = CombinationAxis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color1);

    MaterialInstanceDynamic = CombinationAxis->CreateAndSetMaterialInstanceDynamic(1);
    MaterialInstanceDynamic->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color2);

    CombinationAxis->OnReleased.AddDynamic(this, &ATransformControl::OnAxisReleased);
    CombinationAxis->OnBeginCursorOver.AddDynamic(this, &ATransformControl::OnAxisBeginCursorOver);
    CombinationAxis->OnEndCursorOver.AddDynamic(this, &ATransformControl::OnAxisEndCursorOver);

    CombinationAxis->AttachTo(Center);

    return CombinationAxis;
}

void ATransformControl::UpdateTransformControlPosition()
{
    if (!IsValid(OverlookActor) || !IsValid(AttachedCamera))
    {
        return;
    }

    FRotator ViewRotation = AttachedCamera->GetComponentRotation();
    FVector ViewLocation = AttachedCamera->GetComponentLocation();

    FVector moveToolLocation = (OverlookActor->GetActorLocation() - ViewLocation).Rotation().RotateVector(FVector(61.0f, 0.0f, 0.0f));
    SetActorLocation(ViewLocation + moveToolLocation);
}

void ATransformControl::SetCurrentStatus(EToolStatusEnum Status)
{
    CurrentStatus = Status;
}

EToolStatusEnum ATransformControl::GetCurrentStatus()
{
    return CurrentStatus;
}

void ATransformControl::AutoSwitchNextMode()
{
    switch (GetCurrentMode())
    {
    case EToolModeEnum::EM_MOVE:
        SetCurrentMode(EToolModeEnum::EM_ROTATE);
        break;
    case EToolModeEnum::EM_ROTATE:
        SetCurrentMode(EToolModeEnum::EM_SCALE);
        break;
    case EToolModeEnum::EM_SCALE:
        SetCurrentMode(EToolModeEnum::EM_MOVE);
        break;
    default:
        break;
    }
}

void ATransformControl::SetCurrentMode(EToolModeEnum Mode)
{
    CurrentMode = Mode;

    SwitchControl(CurrentMode);
}

EToolModeEnum ATransformControl::GetCurrentMode()
{
    return CurrentMode;
}

void ATransformControl::SwitchControl(EToolModeEnum mode)
{
    HideAllMeshComponents();
    SetAxisMeshComponentEnable(ControlCenter, true);
    switch (mode)
    {
    case EToolModeEnum::EM_MOVE:
        SetAxisMeshComponentEnable(MoveToolAxisX, true);
        SetAxisMeshComponentEnable(MoveToolAxisY, true);
        SetAxisMeshComponentEnable(MoveToolAxisZ, true);
        SetAxisMeshComponentEnable(MoveToolAxisXZ, true);
        SetAxisMeshComponentEnable(MoveToolAxisXY, true);
        SetAxisMeshComponentEnable(MoveToolAxisYZ, true);
        break;
    case EToolModeEnum::EM_ROTATE:
        SetAxisMeshComponentEnable(RotateToolPitch, true);
        SetAxisMeshComponentEnable(RotateToolRoll, true);
        SetAxisMeshComponentEnable(RotateToolYaw, true);
        SetAxisMeshComponentEnable(ControlCenter, false);
        break;
    case EToolModeEnum::EM_SCALE:
        SetAxisMeshComponentEnable(ScaleToolAxisX, true);
        SetAxisMeshComponentEnable(ScaleToolAxisY, true);
        SetAxisMeshComponentEnable(ScaleToolAxisZ, true);
        SetAxisMeshComponentEnable(ScaleToolAxisXY, true);
        SetAxisMeshComponentEnable(ScaleToolAxisXZ, true);
        SetAxisMeshComponentEnable(ScaleToolAxisYZ, true);
        break;
    default:
        break;
    }
}

void ATransformControl::SetAxisMeshComponentEnable(class UStaticMeshComponent* MeshComponent, bool bEnable)
{
    MeshComponent->SetVisibility(bEnable);
    bEnable ? MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly) : MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetCollisionProfileName(FName("UI"));
}

void ATransformControl::HideAllMeshComponents()
{
    TArray<UActorComponent*> Comps;

    GetComponents(Comps);

    for (int32 index = 0; index < Comps.Num(); ++index)
    {
        UStaticMeshComponent* MeshComp = Cast<UStaticMeshComponent>(Comps[index]);

        if (nullptr == MeshComp)
        {
            continue;
        }
        SetAxisMeshComponentEnable(MeshComp, false);
    }
}

void ATransformControl::OnAxisReleased(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_NONE);
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisReleased");
}

void ATransformControl::OnAxisBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);
}

void ATransformControl::OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);
}

void ATransformControl::SwitchMouseCursor(EMouseCursor::Type type)
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = type;
}

void ATransformControl::OnMoveTooXClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisXClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXMOVE);
}

void ATransformControl::OnMoveToolYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisYClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISYMOVE);
}

void ATransformControl::OnMoveToolZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISZMOVE);
}

void ATransformControl::OnMoveToolXYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisXYClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXYMOVE);
}

void ATransformControl::OnMoveToolXZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisXZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXZMOVE);
}

void ATransformControl::OnMoveToolYZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnMoveToolAxisYZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISYZMOVE);
}


void ATransformControl::OnRotateToolPitchClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnRotateToolPitchClicked");
    SetCurrentStatus(EToolStatusEnum::ES_PITCH);
}

void ATransformControl::OnRotateToolRollClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnRotateToolRollClicked");
    SetCurrentStatus(EToolStatusEnum::ES_ROLL);
}

void ATransformControl::OnRotateToolYawClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnRotateToolYawClicked");
    SetCurrentStatus(EToolStatusEnum::ES_YAW);
}

void ATransformControl::OnScaleToolXClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolXClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXSCALE);
}

void ATransformControl::OnScaleToolYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolYClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISYSCALE);
}

void ATransformControl::OnScaleToolZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISZSCALE);
}

void ATransformControl::OnScaleToolXYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolXYClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXYSCALE);
}

void ATransformControl::OnScaleToolXZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolXZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISXZSCALE);
}

void ATransformControl::OnScaleToolYZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnScaleToolYZClicked");
    SetCurrentStatus(EToolStatusEnum::ES_AXISYZCALE);
}
