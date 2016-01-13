# UE4TransformTool
An UE4 Actor class which simulate the UE4Editor TransformTool
# To Begin
> Spawn tool in world,and bind to your character's camera component

```C++
YourTransformTool = GetWorld()->SpawnActor<ATransformControl>(ATransformControl::StaticClass());
YourTransformTool->AttachToCamera(YourCharacterCameraComponent);
```

> Then use `ATransformControl::SetOverlookActor(class AActor* Actor)` function to show tool on the actor

#EToolStatusEnum
> class `EToolStatusEnum` is a enum that can tell you which axis clicked,
> you can use `ATransformControl::GetCurrentStatus()` function to get current status

#EToolModeEnum
> class `EToolModeEnum` is a enum that can tell you which mode TransformTool is using,
> you can use `ATransformControl::AutoSwitchNextMode()` function to switch mode

# License
MIT