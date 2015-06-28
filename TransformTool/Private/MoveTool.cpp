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

void AMoveTool::BindingTool(class UCameraComponent* Camera)
{
    AttachedCamera = Camera;
}

void AMoveTool::SetOverlookActor(class AActor* Actor)
{
    OverlookActor = Actor;
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
    BoxX->OnBeginCursorOver.AddDynamic(this, &AMoveTool::OnAxisXBeginCursorOver);
    BoxX->OnEndCursorOver.AddDynamic(this, &AMoveTool::OnAxisXEndCursorOver);

    BoxY = CreateBox("BoxY", FVector(0.0f, 4.5f, 0.0f), FVector(0.015625f, 0.1f, 0.015625f));
    BoxY->OnClicked.AddDynamic(this, &AMoveTool::OnAxisYClicked);
    BoxY->OnBeginCursorOver.AddDynamic(this, &AMoveTool::OnAxisYBeginCursorOver);
    BoxY->OnEndCursorOver.AddDynamic(this, &AMoveTool::OnAxisYEndCursorOver);

    BoxZ = CreateBox("BoxZ", FVector(0.0f, 0.0f, 4.5f), FVector(0.015625f, 0.015625f, 0.1f));
    BoxZ->OnClicked.AddDynamic(this, &AMoveTool::OnAxisZClicked);
    BoxZ->OnBeginCursorOver.AddDynamic(this, &AMoveTool::OnAxisZBeginCursorOver);
    BoxZ->OnEndCursorOver.AddDynamic(this, &AMoveTool::OnAxisZEndCursorOver);
}

class UBoxComponent* AMoveTool::CreateBox(FString Name, FVector Location, FVector Scale3D)
{
    UBoxComponent* pBox = CreateDefaultSubobject<UBoxComponent>(*Name);
    pBox->SetCollisionProfileName(FName(TEXT("UI")));

    pBox->RelativeLocation = Location;
    pBox->RelativeScale3D = Scale3D;

    pBox->AttachTo(Center);

    return pBox;
}

void AMoveTool::InitCombinationAxis()
{
    AxisXZ = CreateCombinationAxis("AxisX&Z", FVector(1.4f, 0.0f, 1.4f), FVector(0.024f, 0.000244f, 0.024f));
    AxisYZ = CreateCombinationAxis("AxisY&Z", FVector(0.0f, 1.4f, 1.4f), FVector(0.000244f, 0.024f, 0.024f));
    AxisXY = CreateCombinationAxis("AxisX&Y", FVector(1.4f, 1.4f, 0.0f), FVector(0.024f, 0.024f, 0.000244f));
}

class UStaticMeshComponent* AMoveTool::CreateCombinationAxis(FString Name, FVector Location, FVector Scale3D)
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MehFinder(TEXT("/Engine/BasicShapes/Cube"));

    static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

    UStaticMeshComponent* pCombinationAxis = CreateDefaultSubobject<UStaticMeshComponent>(*Name);

    pCombinationAxis->StaticMesh = MehFinder.Object;

    pCombinationAxis->SetMaterial(0, MatFinder.Object);

    pCombinationAxis->SetCollisionProfileName(FName(TEXT("UI")));

    pCombinationAxis->RelativeLocation = Location;

    pCombinationAxis->RelativeScale3D = Scale3D;

    pCombinationAxis->AttachTo(Center);

    return pCombinationAxis;
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

void AMoveTool::OnAxisXClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisXClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISX);
}

void AMoveTool::OnAxisXReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisXReleased");

    SetCurrentStatus(EMoveToolStatusEnum::ES_NONE);
}

void AMoveTool::OnAxisXBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);
}

void AMoveTool::OnAxisXEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);
}

void AMoveTool::OnAxisYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisYClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISY);
}

void AMoveTool::OnAxisYReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisYReleased");

    SetCurrentStatus(EMoveToolStatusEnum::ES_NONE);
}

void AMoveTool::OnAxisYBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);
}

void AMoveTool::OnAxisYEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);
}

void AMoveTool::OnAxisZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisZClicked");
    SetCurrentStatus(EMoveToolStatusEnum::ES_AXISZ);
}

void AMoveTool::OnAxisZReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "OnAxisZReleased");

    SetCurrentStatus(EMoveToolStatusEnum::ES_NONE);
}

void AMoveTool::OnAxisZBeginCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::CardinalCross);
}

void AMoveTool::OnAxisZEndCursorOver(class UPrimitiveComponent* TouchedComponent)
{
    SwitchMouseCursor(EMouseCursor::Type::Default);
}

void AMoveTool::SetCurrentStatus( EMoveToolStatusEnum Status)
{
    CurrentStatus = Status;
}

void AMoveTool::SwitchMouseCursor(EMouseCursor::Type type)
{
    UGameplayStatics::GetPlayerController(GetWorld(), 0)->CurrentMouseCursor = type;
}

class AActor* AMoveTool::GetOverLookActor()
{
    return OverlookActor;
}

EMoveToolStatusEnum AMoveTool::GetCurrentStatus()
{
    return CurrentStatus;
}
