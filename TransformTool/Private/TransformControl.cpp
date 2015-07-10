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

    InitAxes();

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

void ATransformControl::InitAxes()
{
    InitMoveToolAxes();

    InitRotateToolAxes();

    InitScaleToolAxes();
}

void ATransformControl::InitMoveToolAxes()
{
    ControlCenter = CreateAxis("Center", "MoveTool", FColor(0xFF, 0xFF, 0xFF, 0xFF), EToolStatusEnum::ES_AXES_CENTER);

    UStaticMeshComponent* MoveToolAxisX = CreateAxis("AxisX", "MoveTool", FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_AXISX_MOVE);

    UStaticMeshComponent* MoveToolAxisY = CreateAxis("AxisY", "MoveTool", FColor(0x00, 0xFF, 0x00, 0xFF), EToolStatusEnum::ES_AXISY_MOVE);

    UStaticMeshComponent* MoveToolAxisZ = CreateAxis("AxisZ", "MoveTool", FColor(0x00, 0x00, 0xFF, 0xFF), EToolStatusEnum::ES_AXISZ_MOVE);

    UStaticMeshComponent* MoveToolAxisXY = CreateCombinationAxis("AxisXY", "MoveTool",
                                           FColor(0xFF, 0x00, 0x00, 0xFF), FColor(0x00, 0xFF, 0x00, 0xFF), EToolStatusEnum::ES_AXISXY_MOVE);

    UStaticMeshComponent* MoveToolAxisXZ = CreateCombinationAxis("AxisXZ", "MoveTool",
                                           FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_AXISXZ_MOVE);

    UStaticMeshComponent* MoveToolAxisYZ = CreateCombinationAxis("AxisYZ", "MoveTool",
                                           FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0x00, 0xFF, 0x00, 0xFF), EToolStatusEnum::ES_AXISYZ_MOVE);

    MoveToolAxes.Add(MoveToolAxisX);
    MoveToolAxes.Add(MoveToolAxisY);
    MoveToolAxes.Add(MoveToolAxisZ);
    MoveToolAxes.Add(MoveToolAxisXY);
    MoveToolAxes.Add(MoveToolAxisXZ);
    MoveToolAxes.Add(MoveToolAxisYZ);
}

void ATransformControl::InitRotateToolAxes()
{
    UStaticMeshComponent* RotateToolPitch = CreateAxis("Pitch", "RotateTool", FColor(0x00, 0xFF, 0x00, 0xFF),  EToolStatusEnum::ES_PITCH);

    UStaticMeshComponent* RotateToolRoll = CreateAxis("Roll", "RotateTool", FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_ROLL);

    UStaticMeshComponent* RotateToolYaw = CreateAxis("Yaw", "RotateTool", FColor(0x00, 0x00, 0xFF, 0xFF), EToolStatusEnum::ES_YAW);

    RotateToolAxes.Add(RotateToolPitch);
    RotateToolAxes.Add(RotateToolRoll);
    RotateToolAxes.Add(RotateToolYaw);
}

void ATransformControl::InitScaleToolAxes()
{
    UStaticMeshComponent* ScaleToolAxisX = CreateAxis("AxisX", "ScaleTool", FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_AXISX_SCALE);

    UStaticMeshComponent* ScaleToolAxisY = CreateAxis("AxisY", "ScaleTool", FColor(0x00, 0xFF, 0x00, 0xFF), EToolStatusEnum::ES_AXISY_SCALE);

    UStaticMeshComponent* ScaleToolAxisZ = CreateAxis("AxisZ", "ScaleTool", FColor(0x00, 0x00, 0xFF, 0xFF), EToolStatusEnum::ES_AXISZ_SCALE);

    UStaticMeshComponent* ScaleToolAxisXY = CreateCombinationAxis("AxisXY", "ScaleTool",
                                            FColor(0x00, 0xFF, 0x00, 0xFF), FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_AXISXY_SCALE);

    UStaticMeshComponent* ScaleToolAxisXZ = CreateCombinationAxis("AxisXZ", "ScaleTool",
                                            FColor(0x00, 0x00, 0xFF, 0xFF), FColor(0xFF, 0x00, 0x00, 0xFF), EToolStatusEnum::ES_AXISXZ_SCALE);

    UStaticMeshComponent* ScaleToolAxisYZ = CreateCombinationAxis("AxisYZ", "ScaleTool",
                                            FColor(0x00, 0xFF, 0x00, 0xFF), FColor(0x00, 0x00, 0xFF, 0xFF), EToolStatusEnum::ES_AXISYZ_SCALE);

    ScaleToolAxes.Add(ScaleToolAxisX);
    ScaleToolAxes.Add(ScaleToolAxisY);
    ScaleToolAxes.Add(ScaleToolAxisZ);
    ScaleToolAxes.Add(ScaleToolAxisXY);
    ScaleToolAxes.Add(ScaleToolAxisXZ);
    ScaleToolAxes.Add(ScaleToolAxisYZ);
}

class UStaticMeshComponent* ATransformControl::CreateAxis(FString Name, FString Group,
            FColor Color, EToolStatusEnum ToolStatus)
{
    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));

    UStaticMeshComponent* Axis = CreateDefaultSubobject<UStaticMeshComponent>(*(Group + Name));
    Axis->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/" + Group + "/" + Name + "." + Name));
    Axis->SetCollisionProfileName(FName("UI"));
    Axis->bCastDynamicShadow = false;

    Axis->SetMaterial(0, Mat);

    UMaterialInstanceDynamic* MaterialInstanceDynamic = Axis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color);

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add(MaterialInstanceDynamic);

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add(Color);

    Axis->OnClicked.AddDynamic(this, &ATransformControl::OnAxisClicked);
    Axis->OnReleased.AddDynamic(this, &ATransformControl::OnAxisReleased);
    Axis->OnBeginCursorOver.AddDynamic(this, &ATransformControl::OnAxisBeginCursorOver);
    Axis->OnEndCursorOver.AddDynamic(this, &ATransformControl::OnAxisEndCursorOver);

    Axis->AttachTo(Center);

    AxisStatusMap.Add(Axis, ToolStatus);
    MaterialColorMap.Add(Axis, MaterialColorArray);
    MaterialInstanceMap.Add(Axis, MaterialInstanceArray);

    return Axis;
}

class UStaticMeshComponent* ATransformControl::CreateCombinationAxis(FString Name, FString Group,
            FColor Color1, FColor Color2, EToolStatusEnum ToolStatus)
{
    UMaterial* Mat = LoadObject<UMaterial>(NULL, TEXT("/Game/TransformTool/GizmoMaterial.GizmoMaterial"));

    UStaticMeshComponent* CombinationAxis = CreateDefaultSubobject<UStaticMeshComponent>(*(Group + Name));
    CombinationAxis->StaticMesh = LoadObject<UStaticMesh>(NULL, *FString("/Game/TransformTool/" + Group + "/" + Name + "." + Name));
    CombinationAxis->SetCollisionProfileName(FName("UI"));
    CombinationAxis->bCastDynamicShadow = false;

    CombinationAxis->SetMaterial(0, Mat);
    CombinationAxis->SetMaterial(1, Mat);

    UMaterialInstanceDynamic* MaterialInstanceDynamic1 = CombinationAxis->CreateAndSetMaterialInstanceDynamic(0);
    MaterialInstanceDynamic1->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color1);

    UMaterialInstanceDynamic* MaterialInstanceDynamic2 = CombinationAxis->CreateAndSetMaterialInstanceDynamic(1);
    MaterialInstanceDynamic2->SetVectorParameterValue(FName(TEXT("GizmoColor")), Color2);

    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray;
    MaterialInstanceArray.Add(MaterialInstanceDynamic1);
    MaterialInstanceArray.Add(MaterialInstanceDynamic2);

    TArray<FColor> MaterialColorArray;
    MaterialColorArray.Add(Color1);
    MaterialColorArray.Add(Color2);

    CombinationAxis->OnClicked.AddDynamic(this, &ATransformControl::OnAxisClicked);
    CombinationAxis->OnReleased.AddDynamic(this, &ATransformControl::OnAxisReleased);
    CombinationAxis->OnBeginCursorOver.AddDynamic(this, &ATransformControl::OnAxisBeginCursorOver);
    CombinationAxis->OnEndCursorOver.AddDynamic(this, &ATransformControl::OnAxisEndCursorOver);

    CombinationAxis->AttachTo(Center);

    AxisStatusMap.Add(CombinationAxis, ToolStatus);
    MaterialColorMap.Add(CombinationAxis, MaterialColorArray);
    MaterialInstanceMap.Add(CombinationAxis, MaterialInstanceArray);

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
	
	if (EToolModeEnum::EM_SCALE == GetCurrentMode())
    {
        SetActorRotation(OverlookActor->GetActorRotation());
    }
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
        SetAxesMeshComponentEnable(MoveToolAxes, true);
        break;
    case EToolModeEnum::EM_ROTATE:
        SetAxesMeshComponentEnable(RotateToolAxes, true);
        SetAxisMeshComponentEnable(ControlCenter, false);
        break;
    case EToolModeEnum::EM_SCALE:
        SetAxesMeshComponentEnable(ScaleToolAxes, true);
        break;
    default:
        break;
    }
}

void ATransformControl::SetAxesMeshComponentEnable(const TArray<UStaticMeshComponent*> &AxesMesh, bool bEnable)
{
    for (int32 index = 0; index < AxesMesh.Num(); ++index)
    {
        SetAxisMeshComponentEnable(AxesMesh[index], bEnable);
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

void ATransformControl::OnAxisClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "Clicked: " + TouchedComponent->GetName());

    SetCurrentStatus(*AxisStatusMap.Find(Cast<UStaticMeshComponent>(TouchedComponent)));
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

    SetAxisCursorOverColor(Cast<UStaticMeshComponent>(TouchedComponent));
}

void ATransformControl::OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);

    if (GetCurrentStatus() != EToolStatusEnum::ES_NONE)
    {
        return;
    }

    RecoverAxisColor(Cast<UStaticMeshComponent>(TouchedComponent));
}

void ATransformControl::SwitchMouseCursor(EMouseCursor::Type type)
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = type;
}

void ATransformControl::SetAxisCursorOverColor(UStaticMeshComponent* Axis)
{
    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray = *MaterialInstanceMap.Find(Axis);
    for (int32 index = 0; index < MaterialInstanceArray.Num(); ++index)
    {
        MaterialInstanceArray[index]->SetVectorParameterValue(FName(TEXT("GizmoColor")), FColor(0xFF, 0xFF, 0x00, 0xFF));
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "SetAxisCursorOverColor: " + Axis->GetName());
}

void ATransformControl::RecoverAxisColor(UStaticMeshComponent* Axis)
{
    TArray<UMaterialInstanceDynamic*> MaterialInstanceArray = *MaterialInstanceMap.Find(Axis);
    TArray<FColor> MaterialColorArray = *MaterialColorMap.Find(Axis);
    for (int32 index = 0; index < MaterialInstanceArray.Num(); ++index)
    {
        MaterialInstanceArray[index]->SetVectorParameterValue(FName(TEXT("GizmoColor")), MaterialColorArray[index]);
    }

    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Blue, "RecoverAxisColor");
}