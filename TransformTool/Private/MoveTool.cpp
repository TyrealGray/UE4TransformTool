// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
#include "TransformToolPrivatePCH.h"
#include "MoveTool.h"

// Sets default values
AMoveTool::AMoveTool()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    CurrentStatus = EMoveToolStatusEnum::ES_NONE;

    AttachedCamera = nullptr;

    OverlookActor = nullptr;

    Center = CreateDefaultSubobject<USphereComponent>(TEXT("center"));
    RootComponent = Center;
    Center->SetSphereRadius(0.2f);

    InitAxis();

    InitBox();

    InitCombinationAxis();
}

// Called when the game starts or when spawned
void AMoveTool::BeginPlay()
{
    Super::BeginPlay();
}

// Called every frame
void AMoveTool::Tick( float DeltaTime )
{
    Super::Tick( DeltaTime );
}

void AMoveTool::AttachToCamera(class UCameraComponent* Camera)
{
    AttachedCamera = Camera;
}

void AMoveTool::SetOverlookActor(class AActor* Actor)
{
    OverlookActor = (this == Actor) ? OverlookActor : Actor;
}

void AMoveTool::InitAxis()
{
    AxisX = CreateAxis("AxisX", FColor(0xFF, 0x00, 0x00, 0xFF), FRotator(0.0f, 0.0f, 90.0f));

    AxisY = CreateAxis("AxisY", FColor(0x00, 0xFF, 0x00, 0xFF), FRotator(0.0f, 90.0f, 0.0f));

    AxisZ = CreateAxis("AxisZ", FColor(0x00, 0x00, 0xFF, 0xFF), FRotator(90.0f, 0.0f, 0.0f));
}

class UArrowComponent* AMoveTool::CreateAxis(FString Name, FColor Color, FRotator Rotation)
{
    UArrowComponent* pAxis = CreateDefaultSubobject<UArrowComponent>(*Name);

    pAxis->ArrowSize = 0.1f;
    pAxis->bHiddenInGame = false;
    pAxis->ArrowColor = Color;
    pAxis->RelativeRotation = Rotation;

    pAxis->AttachTo(Center);

    return pAxis;
}

void AMoveTool::InitBox()
{
    BoxX = CreateBox("BoxX", FVector(4.5f, 0.0f, 0.0f), FVector(0.1f, 0.015625f, 0.015625f));
    BoxX->OnClicked.AddDynamic(this, &AMoveTool::OnAxisXClicked);

    BoxY = CreateBox("BoxY", FVector(0.0f, 4.5f, 0.0f), FVector(0.015625f, 0.1f, 0.015625f));
    BoxY->OnClicked.AddDynamic(this, &AMoveTool::OnAxisYClicked);

    BoxZ = CreateBox("BoxZ", FVector(0.0f, 0.0f, 4.5f), FVector(0.015625f, 0.015625f, 0.1f));
    BoxZ->OnClicked.AddDynamic(this, &AMoveTool::OnAxisZClicked);
}

class UBoxComponent* AMoveTool::CreateBox(FString Name, FVector Location, FVector Scale3D)
{
    UBoxComponent* Box = CreateDefaultSubobject<UBoxComponent>(*Name);
    Box->SetCollisionProfileName(FName(TEXT("UI")));

    Box->RelativeLocation = Location;
    Box->RelativeScale3D = Scale3D;

    Box->OnReleased.AddDynamic(this, &AMoveTool::OnAxisReleased);
    Box->OnBeginCursorOver.AddDynamic(this, &AMoveTool::OnAxisBeginCursorOver);
    Box->OnEndCursorOver.AddDynamic(this, &AMoveTool::OnAxisEndCursorOver);

    Box->AttachTo(Center);

    return Box;
}

void AMoveTool::InitCombinationAxis()
{
    AxisXY = CreateCombinationAxis("AxisX&Y", FVector(1.4f, 1.4f, 0.0f), FVector(0.024f, 0.024f, 0.000244f));
    AxisXY->OnClicked.AddDynamic(this, &AMoveTool::OnAxisXYClicked);

    AxisXZ = CreateCombinationAxis("AxisX&Z", FVector(1.4f, 0.0f, 1.4f), FVector(0.024f, 0.000244f, 0.024f));
    AxisXZ->OnClicked.AddDynamic(this, &AMoveTool::OnAxisXZClicked);

    AxisYZ = CreateCombinationAxis("AxisY&Z", FVector(0.0f, 1.4f, 1.4f), FVector(0.000244f, 0.024f, 0.024f));
    AxisYZ->OnClicked.AddDynamic(this, &AMoveTool::OnAxisYZClicked);
}

class UStaticMeshComponent* AMoveTool::CreateCombinationAxis(FString Name, FVector Location, FVector Scale3D)
{
    UStaticMeshComponent* CombinationAxis = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MehFinder(TEXT("/Engine/BasicShapes/Cube"));
    static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

    CombinationAxis->StaticMesh = MehFinder.Object;
    CombinationAxis->SetMaterial(0, MatFinder.Object);

    CombinationAxis->SetCollisionProfileName(FName(TEXT("UI")));

    CombinationAxis->RelativeLocation = Location;
    CombinationAxis->RelativeScale3D = Scale3D;

    CombinationAxis->OnReleased.AddDynamic(this, &AMoveTool::OnAxisReleased);
    CombinationAxis->OnBeginCursorOver.AddDynamic(this, &AMoveTool::OnAxisBeginCursorOver);
    CombinationAxis->OnEndCursorOver.AddDynamic(this, &AMoveTool::OnAxisEndCursorOver);

    CombinationAxis->AttachTo(Center);

    return CombinationAxis;
}

void AMoveTool::UpdateMoveToolPosition()
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

void AMoveTool::SetCurrentStatus(EMoveToolStatusEnum Status)
{
    CurrentStatus = Status;
}

class AActor* AMoveTool::GetOverLookActor()
{
    return OverlookActor;
}

EMoveToolStatusEnum AMoveTool::GetCurrentStatus()
{
    return CurrentStatus;
}

void AMoveTool::SwitchMouseCursor(EMouseCursor::Type type)
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = type;
}

void AMoveTool::OnAxisXClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisXClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISX);
}

void AMoveTool::OnAxisYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisYClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISY);
}

void AMoveTool::OnAxisZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisZClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISZ);
}

void AMoveTool::OnAxisXYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisXYClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISXY);
}

void AMoveTool::OnAxisXZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisXZClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISXZ);
}

void AMoveTool::OnAxisYZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisYZClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISYZ);
}

void AMoveTool::OnAxisReleased(class UPrimitiveComponent* TouchedComponent)
{
    SetCurrentStatus(EMoveToolStatusEnum::ES_NONE);
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisReleased");
}

void AMoveTool::OnAxisBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);
}

void AMoveTool::OnAxisEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);
}
