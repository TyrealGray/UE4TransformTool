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
    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));

    UStaticMeshComponent* Axis = CreateDefaultSubobject<UStaticMeshComponent>(*(Group + Name));

    Axis->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/" + Group + "/" + Name + "." + Name));
    Axis->bCastDynamicShadow = false;

    Axis->SetMaterial(0, Mat);

    UMaterialInstanceDynamic* MaterialInstanceDynamic = Axis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color);

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add(MaterialInstanceDynamic);
    MaterialInstanceMap.Add((Group + Name), MaterialInstanceArray);

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add(Color);
    MaterialColorMap.Add((Group + Name), MaterialColorArray);

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
    CombinationAxis->bCastDynamicShadow = false;

    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));
    CombinationAxis->SetMaterial(0, Mat);
    CombinationAxis->SetMaterial(1, Mat);

    UMaterialInstanceDynamic* MaterialInstanceDynamic1 = CombinationAxis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic1->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color1);

    UMaterialInstanceDynamic* MaterialInstanceDynamic2 = CombinationAxis->CreateAndSetMaterialInstanceDynamic(1);
    MaterialInstanceDynamic2->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color2);

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add(MaterialInstanceDynamic1);
    MaterialInstanceArray.Add(MaterialInstanceDynamic2);

    MaterialInstanceMap.Add((Group + Name), MaterialInstanceArray);

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add(Color1);
    MaterialColorArray.Add(Color2);

    MaterialColorMap.Add((Group + Name), MaterialColorArray);

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
    MeshComponent->SetCollisionProfileName(FName("UI"));
    bEnable ? MeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly) : MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MeshComponent->SetVisibility(bEnable);

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
}

void ATransformControl::OnAxisBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);

    if (GetCurrentStatus() != EToolStatusEnum::ES_NONE)
    {
        return;
    }

    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);

    if (GetCurrentStatus() != EToolStatusEnum::ES_NONE)
    {
        return;
    }

    RecoverAxisColor(TouchedComponent->GetName());
}

void ATransformControl::SwitchMouseCursor(EMouseCursor::Type type)
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = type;
}

void ATransformControl::OnMoveTooXClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISX_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnMoveToolYClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISY_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnMoveToolZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISZ_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnMoveToolXYClicked(class UPrimitiveComponent* TouchedComponent)
{

    SetCurrentStatus(EToolStatusEnum::ES_AXISXY_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnMoveToolXZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISXZ_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnMoveToolYZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISYZ_MOVE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}


void ATransformControl::OnRotateToolPitchClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_PITCH);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnRotateToolRollClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_ROLL);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnRotateToolYawClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_YAW);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolXClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISX_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolYClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISY_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISZ_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolXYClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISXY_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolXZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISXZ_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::OnScaleToolYZClicked(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EToolStatusEnum::ES_AXISYZ_SCALE);
    SetAxisCursorOverColor(TouchedComponent->GetName());
}

void ATransformControl::SetAxisCursorOverColor(FString AxisName)
{
    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray = *MaterialInstanceMap.Find(AxisName);
    for (int32 index = 0; index < MaterialInstanceArray.Num(); ++index)
    {
        MaterialInstanceArray[index]->SetVectorParameterValue(FName(TEXT("GizmoColor")), FColor(0xFF, 0xFF, 0x00, 0xFF));
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "SetAxisCursorOverColor: " + AxisName);
}

void ATransformControl::RecoverAxisColor(FString AxisName)
{
    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray = *MaterialInstanceMap.Find(AxisName);
    TArray<FColor> MaterialColorArray = *MaterialColorMap.Find(AxisName);
    for (int32 index = 0; index < MaterialInstanceArray.Num(); ++index)
    {
        MaterialInstanceArray[index]->SetVectorParameterValue(FName(TEXT("GizmoColor")), MaterialColorArray[index]);
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "RecoverAxisColor");
}
