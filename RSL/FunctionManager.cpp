#include "FunctionManager.h"

/* Initializes rfg function pointers. */
void FunctionManager::Initialize()
{
    RegisterFunction(CameraStartSlewMode, 0x2D09B0);
    RegisterFunction(CameraStopSlewMode, 0x2D09C0);
    RegisterFunction(CameraStartThirdPerson, 0x2CF0C0);
    RegisterFunction(CameraStartFirstPerson, 0x2C9AC0);
    RegisterFunction(CameraStopFirstPerson, 0x2C9B60);

    RegisterFunction(HudHide, 0x441A90);

    RegisterFunction(GameRenderSetFogEnabled, 0x3C2C70);

    RegisterFunction(ExplosionCreate, 0x2EC720);
    RegisterFunction(GraphicsBeginFrame, 0x86DD00);

    RegisterFunction(FogOfWarClear, 0x43F7F0);
    RegisterFunction(MouseGenericAutoCentering, 0x1692B0);
    RegisterFunction(CameraViewDataSetView, 0x2D0290);
    RegisterFunction(PlayerDoFrame, 0x6D5A80);
    RegisterFunction(PlayerConstructor, 0x6DECA0);
    RegisterFunction(CharacterControllerSetPos, 0x4153D0);

    RegisterFunction(ObjectUpdatePosAndOrient, 0x68C310);

    RegisterFunction(HavokBodyGetPositionAndOrient, 0x429530);
    RegisterFunction(HavokBodySetPosition, 0x416EA0);

    RegisterFunction(HavokBodySetPositionAndOrient, 0x423700);

    RegisterFunction(HumanUpdatePosAndOrient, 0x69AF70);
    RegisterFunction(HumanTeleportUnsafe, 0x67C660);

    /*Start of MP Detection Hooks*/
    RegisterFunction(IsValidEigenGradient, 0x1D0DD0);
    RegisterFunction(HookDoFrame, 0x3CC750);
    RegisterFunction(InvertDataItem, 0x497740);
    RegisterFunction(AllocatorStillValid, 0x4F50B0);
    RegisterFunction(CsWrapSlice, 0x516D80);
    /*End of MP Detection Hooks*/

    RegisterFunction(KeenGraphicsResizeRenderSwapchain, 0x86AB20);

    RegisterFunction(GameseqGetState, 0x3BFCF0);

    RegisterFunction(ObjectEnableOutline, 0x310C50);
    RegisterFunction(ObjectDisableOutline, 0x310C60);
    RegisterFunction(HumanEnableOutline, 0x62EE80);
    RegisterFunction(HumanDisableOutline, 0x62EF00);

    RegisterFunction(TargetOutlineAddObject, 0x735390);

    RegisterFunction(XrayEffectStart, 0x2E4E30);
    RegisterFunction(XrayEffectStop, 0x2E4E50);

    RegisterFunction(UiToolTipsAddTip, 0x4DA4F0);

    RegisterFunction(ConsoleSetConsoleMode, 0x18CD30);
    RegisterFunction(ConsoleGetConsoleMode, 0x18CD50);
    RegisterFunction(ConsoleInit, 0x1CD150);
    RegisterFunction(ConsoleActivate, 0x18CAC0);
    RegisterFunction(ConsoleDeactivate, 0x1AF950);

    RegisterFunction(RlDrawTristrip2dBegin, 0x10DDA0);
    RegisterFunction(RlDrawString, 0x155BC0);

    RegisterFunction(IsGamePaused, 0x3C1690);
    RegisterFunction(WorldDoFrame, 0x540AB0);
    RegisterFunction(WorldGetObjectName, 0x51C8A0);
    
    RegisterFunction(WorldGetObjectZoneByIndex, 0x1D9F0);
    RegisterFunction(WorldGetObjectZoneByGridId, 0x51DA20);

    RegisterFunction(GsmGetAlertLevel, 0x35D610);
    RegisterFunction(GsmSetAlertLevel, 0x3AA740);
    RegisterFunction(GsmGetAlertValue, 0x35D630);
    RegisterFunction(GsmSetAlertValue, 0x3A44B0);
    RegisterFunction(GsmGetAlertLevelCap, 0x35D680);
    RegisterFunction(GsmSetAlertLevelCap, 0x3AA770);
    
    RegisterFunction(GameRenderGetTodLight, 0x3C2960);
    RegisterFunction(GameRenderSetTodLight, 0x3C2930);
    
    RegisterFunction(GameRenderGetTodOnOffValue, 0x3C3000);
    RegisterFunction(GameRenderSetTodOnOffValue, 0x3C2FF0);
    
    RegisterFunction(GameRenderSetTodLightColor, 0x3C2990);
    RegisterFunction(GameClockGetTodPartitioned, 0x3C15C0);
    
    RegisterFunction(GameClockGetTimeOfDay, 0x3C15E0);
    RegisterFunction(GameClockSetTime, 0x3C1770);
    
    RegisterFunction(GameTimeOfDayGetSkyboxTodColors, 0x3DFA10);
    RegisterFunction(GameTimeOfDayGetWeatherSettings, 0x3C5880);
    RegisterFunction(GameTimeOfDayGetCloudScrollValues, 0x3C58C0);
    RegisterFunction(GameTimeOfDayGetNumOrbitals, 0x3D20C0);
    RegisterFunction(GameTimeOfDayGetCloudLighting, 0x3E31B0);
    RegisterFunction(GameTimeOfDayGetOrbitalParams, 0x3E5400);
    
    RegisterFunction(DistrictFromName, 0x46C4D0);
    
    RegisterFunction(GetWorldZoneByIndex, 0x51D950);
    RegisterFunction(GetWorldZoneByGridId, 0x51D970);
    RegisterFunction(GetWorldZoneByName, 0x51D9A0);
    RegisterFunction(GetWorldZoneByPos, 0x523BA0);
    RegisterFunction(GetWorldZoneByObjectHandle, 0x523C00);
    
    RegisterFunction(RfgDynApplyPointImpulse, 0x3124D0);
    RegisterFunction(RfgDynRepairSphere, 0x314820);
    
    RegisterFunction(RlCameraRenderBegin, 0x137660);

    RegisterFunction(GameRenderSetFarClipDistance, 0x3C2C40);
    RegisterFunction(GameRenderGetFarClipDistance, 0x3C2C50);
    RegisterFunction(GameRenderGetMainScene, 0x3C2920);

    RegisterFunction(hkpWorldStepDeltaTime, 0x9E1A70);
    
    RegisterFunction(ApplicationUpdateTime, 0x5A880);
    
    RegisterFunction(hkpWorldGetGravity, 0x40D040);
    RegisterFunction(hkpWorldSetGravity, 0x9E0E20);
    
    RegisterFunction(VintGetLuaState, 0x620F0);
    RegisterFunction(LuaDoBuffer, 0x82FD20);
    
    RegisterFunction(HavokBodyGetPointer, 0x40E870);
    RegisterFunction(HavokBodyApplyLinearImpulseA, 0x4177A0);
    RegisterFunction(HavokBodyApplyLinearImpulseB, 0x423C20);
    
    RegisterFunction(KeenGraphicsBeginFrame, 0x86DD00);
    
    RegisterFunction(KeenGetBuildVersionString, 0x58740);
    
    RegisterFunction(WorldObjectFromHandle, 0x53C050);

    RegisterFunction(UnusedDcfRagdollPlayer, 0x6D4F30);

    RegisterFunction(StreamGridSetStreamRadius, 0x51AEB0);
    RegisterFunction(StreamGridGetStreamRadius, 0x51AFB0);
    RegisterFunction(StreamGridDoFrame, 0x530FB0);

    RegisterFunction(CutsceneSpawnVehicle, 0x305D70);
    //RegisterFunction(ObjectSpawnVehicle, 0x757F40);
    RegisterFunction(VehicleInfoGetMasterList, 0x768E40);
    RegisterFunction(SpSpawnVehicle, 0x756420);

    RegisterFunction(create_new_vehicle_raw, 0x748960);

    RegisterFunction(load_level_internal, 0x4895F0);
    RegisterFunction(WorldSetPendingTerritoryLoad, 0x51C4A0);

    RegisterFunction(peg_load_wrapper, 0x1D1F10);

    RegisterFunction(string_hash, 0x18FE90);
}