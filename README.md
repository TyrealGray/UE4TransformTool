# UE4TransformTool
An UE4.8 Actor class which simulate the UE4Editor TransformTool
# To Begin
> Spawn tool in world,and bind to your character's camera component
>```C++
GetWorld()->SpawnActor<AMoveTool>(AMoveTool::StaticClass(), FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f));
AttachToCamera(YourCharacterCameraComponent);
```
> Then add `AMoveTool::UpdateMoveToolPosition()` function to character `Tick(float DeltaTime )` function,and use `AMoveTool::SetOverlookActor(class AActor* Actor)` function to show tool on the actor
#License
MIT