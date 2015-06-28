# UE4TransformTool
An UE4.8 Actor class which simulate the UE4Editor TransformTool
# To Begin
> Spawn tool in world,and bind to your character's camera component
>```C++
YourMoveTool = GetWorld()->SpawnActor<AMoveTool>(AMoveTool::StaticClass());
YourMoveTool->AttachToCamera(YourCharacterCameraComponent);
```
> Then add `AMoveTool::UpdateMoveToolPosition()` function to character `Tick(float DeltaTime )` function,and use `AMoveTool::SetOverlookActor(class AActor* Actor)` function to show tool on the actor

#EMoveToolStatusEnum
class `EMoveToolStatusEnum` is a enum that can tell you which axis clicked
>ES_NONE UMETA(DisplayName = "None")
ES_AXISX 	UMETA(DisplayName = "AxisX")
ES_AXISY	UMETA(DisplayName = "AxisY")
ES_AXISZ	UMETA(DisplayName = "AxisZ")
ES_AXISXY	UMETA(DisplayName = "AxisXY")
ES_AXISXZ	UMETA(DisplayName = "AxisXZ")
ES_AXISYZ	UMETA(DisplayName = "AxisYZ")

you can use `AMoveTool::GetCurrentStatus()` function to get current status


# License
MIT