using UnrealBuildTool;
public class TransformTool : ModuleRules
{
   public TransformTool(TargetInfo Target)
   {
       PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

       PrivateIncludePaths.AddRange(new string[] { "TransformTool/Private" });

       PublicIncludePaths.AddRange(new string[] { "TransformTool/Public" });
   }
}