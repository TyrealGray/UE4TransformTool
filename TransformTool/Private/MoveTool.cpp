// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
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

    //m_pCenter->OnBeginCursorOver.AddDynamic();

    InitAxis();

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

    UpdateMoveToolPosition();
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

    pAxis->ArrowSize = 0.025f;
    pAxis->bHiddenInGame = false;

    pAxis->AttachTo(Center);

    return pAxis;
}

void AMoveTool::InitCombinationAxis()
{
    AxisXZ = CreateCombinationAxis("AxisX&Z");
    AxisYZ = CreateCombinationAxis("AxisY&Z");
    AxisXY = CreateCombinationAxis("AxisX&Y");

    AxisXZ->RelativeLocation = FVector(0.25f, 0.0f, 0.25f);
    AxisXZ->RelativeScale3D = FVector(0.003906f, 0.000244f, 0.003906f);

    AxisYZ->RelativeLocation = FVector(0.0f, 0.25f, 0.25f);
    AxisYZ->RelativeScale3D = FVector(0.000244f, 0.003906f, 0.003906f);

    AxisXY->RelativeLocation = FVector(0.25f, 0.25f, 0.0f);
    AxisXY->RelativeScale3D = FVector(0.003906f, 0.003906f, 0.000244f);
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

    FVector moveToolLocation = (OverlookActor->GetActorLocation() - ViewLocation).Rotation().RotateVector(FVector(15.0f, 0.0f, 0.0f));
    SetActorLocation(ViewLocation + moveToolLocation);
}
