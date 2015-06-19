// Fill out your copyright notice in the Description page of Project Settings.

#include "TransformTool.h"
#include "MoveTool.h"


// Sets default values
AMoveTool::AMoveTool()
{
    //static ConstructorHelpers::FObjectFinder<UMaterial> MatFinder(TEXT("/Engine/BasicShapes/BasicShapeMaterial"));

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    m_pCenter = CreateDefaultSubobject<USphereComponent>(TEXT("center"));
    RootComponent = m_pCenter;
    m_pCenter->SetSphereRadius(0.2f);
    //if (MatFinder.Succeeded())
    //{
    //    m_pCenter->ShapeMaterial = UMaterialInstanceDynamic::Create(MatFinder.Object, this);
    //}

    //m_pCenter->OnBeginCursorOver.AddDynamic();

    initAxis();
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

void AMoveTool::initAxis()
{
    m_pAxisX = createAxis("AxisX");
    m_pAxisX->ArrowColor = FColor(0xFF, 0x00, 0x00, 0xFF);
    m_pAxisX->RelativeRotation = FRotator(0.0f, 0.0f, 90.0f);

    m_pAxisY = createAxis("AxisY");
    m_pAxisY->ArrowColor = FColor(0x00, 0xFF, 0x00, 0xFF);
    m_pAxisY->RelativeRotation = FRotator(0.0f, 90.0f, 0.0f);

    m_pAxisZ = createAxis("AxisZ");
    m_pAxisZ->ArrowColor = FColor(0x00, 0x00, 0xFF, 0xFF);
    m_pAxisZ->RelativeRotation = FRotator(90.0f, 0.0f, 0.0f);
}

class UArrowComponent* AMoveTool::createAxis(FString name)
{
    UArrowComponent* pAxis = CreateDefaultSubobject<UArrowComponent>(*name);

    pAxis->ArrowSize = 0.025f;
    pAxis->bHiddenInGame = false;

    pAxis->AttachTo(m_pCenter);

    return pAxis;
}

