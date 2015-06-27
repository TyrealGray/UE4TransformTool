// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
#include "TransformToolPrivatePCH.h"
#include "MoveTool.h"

// Sets default values
AMoveTool::AMoveTool()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

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
    AxisX = CreateAxis("AxisX");
    AxisX->ArrowColor = FColor(0xFF, 0x00, 0x00, 0xFF);
    AxisX->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);

    AxisY = CreateAxis("AxisY");
    AxisY->ArrowColor = FColor(0x00, 0xFF, 0x00, 0xFF);
    AxisY->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);

    AxisZ = CreateAxis("AxisZ");
    AxisZ->ArrowColor = FColor(0x00, 0x00, 0xFF, 0xFF);
    AxisZ->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
}

class UArrowComponent* AMoveTool::CreateAxis(FString name)
{
    UArrowComponent* pAxis = CreateDefaultSubobject<UArrowComponent>(*name);

    pAxis->ArrowSize = 0.1f;
    pAxis->bHiddenInGame = false;

    pAxis->AttachTo(Center);

    return pAxis;
}

void AMoveTool::InitBox()
{
    BoxX = CreateBox("BoxX");
    BoxX->RelativeLocation = FVector(4.5f, 0.0f, 0.0f);
    BoxX->RelativeScale3D = FVector(0.1f, 0.015625f, 0.015625f);

    BoxX->OnClicked.AddDynamic(this, &AMoveTool::OnAxisXClicked);

    BoxY = CreateBox("BoxY");
    BoxY->RelativeLocation = FVector(0.0f, 4.5f, 0.0f);
    BoxY->RelativeScale3D = FVector(0.015625f, 0.1f, 0.015625f);

    BoxY->OnClicked.AddDynamic(this, &AMoveTool::OnAxisYClicked);

    BoxZ = CreateBox("BoxZ");
    BoxZ->RelativeLocation = FVector(0.0f, 0.0f, 4.5f);
    BoxZ->RelativeScale3D = FVector(0.015625f, 0.015625f, 0.1f);

    BoxZ->OnClicked.AddDynamic(this, &AMoveTool::OnAxisZClicked);
}

class UBoxComponent* AMoveTool::CreateBox(FString name)
{
    UBoxComponent* pBox = CreateDefaultSubobject<UBoxComponent>(*name);
    pBox->SetCollisionProfileName(FName(TEXT("UI")));
    pBox->AttachTo(Center);

    return pBox;
}

void AMoveTool::InitCombinationAxis()
{
    AxisXZ = CreateCombinationAxis("AxisX&Z");
    AxisYZ = CreateCombinationAxis("AxisY&Z");
    AxisXY = CreateCombinationAxis("AxisX&Y");

    AxisXZ->RelativeLocation = FVector(0.7f, 0.0f, 0.7f);
    AxisXZ->RelativeScale3D = FVector(0.009765f, 0.000244f, 0.009765f);

    AxisYZ->RelativeLocation = FVector(0.0f, 0.7f, 0.7f);
    AxisYZ->RelativeScale3D = FVector(0.000244f, 0.009765f, 0.009765f);

    AxisXY->RelativeLocation = FVector(0.7f, 0.7f, 0.0f);
    AxisXY->RelativeScale3D = FVector(0.009765f, 0.009765f, 0.000244f);
}

class UStaticMeshComponent* AMoveTool::CreateCombinationAxis(FString name)
{
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MehFinder(TEXT("/Engine/BasicShapes/Cube"));

    static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

    UStaticMeshComponent* pCombinationAxis = CreateDefaultSubobject<UStaticMeshComponent>(*name);

    pCombinationAxis->StaticMesh = MehFinder.Object;

    //pCombinationAxis->SetMaterial(0, MatFinder.Object);

    pCombinationAxis->AttachTo(Center);

    pCombinationAxis->SetCollisionProfileName(FName(TEXT("UI")));

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

    FVector moveToolLocation = (OverlookActor->GetActorLocation() - ViewLocation).Rotation().RotateVector(FVector(72.0f, 0.0f, 0.0f));
    SetActorLocation(ViewLocation + moveToolLocation);
}

void AMoveTool::OnAxisXClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisXClicked");
}

void AMoveTool::OnAxisXReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisXReleased");
}

void AMoveTool::OnAxisYClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisYClicked");
}

void AMoveTool::OnAxisYReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisYReleased");
}

void AMoveTool::OnAxisZClicked(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisZClicked");
}

void AMoveTool::OnAxisZReleased(class UPrimitiveComponent* TouchedComponent)
{
    GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "OnAxisZReleased");
}
