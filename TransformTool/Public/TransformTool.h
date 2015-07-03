#pragma once
#include "ModuleManager.h"
class TransformToolImpl : public IModuleInterface
{
public:
   /** IModuleInterface implementation */
   void StartupModule();
   void ShutdownModule();
};
