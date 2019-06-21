#include "Application.h"

void Application::Run()
{
    Init();
    MainLoop();
    Exit();
}

void Application::Init()
{
    InitLogger();
    CheckForImproperInstallation();
    InitRSL();
    OpenDefaultLogs();
}

void Application::InitLogger()
{
    try
    {
        Logger::Init(LogAll, Globals::GetEXEPath(false) + "RFGR Script Loader/Logs/", 10000);
        Logger::OpenLogFile("Load Log.txt", LogAll, std::ios_base::trunc);
        Logger::Log("RFGR Script Loader started. Activating.", LogInfo, true);
    }
    catch (std::exception& Ex)
    {
        std::string MessageBoxString = R"(Exception detected during Logger initialization! Please show this to the current script loader maintainer. It's much harder to fix any further problems which might occur without logs.)";
        MessageBoxString += Ex.what();
        MessageBoxA(Globals::FindRfgTopWindow(), MessageBoxString.c_str(), "Logger failed to initialize!", MB_OK);
        Logger::CloseAllLogFiles();
    }
}

void Application::CheckForImproperInstallation()
{
    if (IsFolderPlacementError())
    {
        FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
    }
}

void Application::InitRSL()
{
    try
    {
        if (!LoadDataFromConfig())
        {
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            return;
        }
        OpenConsole();

        //Attempt to init kiero which is used for easy directx hooking. Shutdown if it fails.
        if (kiero::init(kiero::RenderType::D3D11) != kiero::Status::Success)
        {
            Logger::Log(std::string("Kiero error: " + std::to_string(kiero::init(kiero::RenderType::D3D11))), LogFatalError, true);
            Logger::Log("Failed to initialize kiero for D3D11. RFGR Script loader deactivating.", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            return;
        }
        if (MH_Initialize() != MH_OK)
        {
            Logger::Log("Failed to initialize MinHook. RFGR Script loader deactivating.", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            return;
        }

        //Set global values which are frequently used in hooks.
        Globals::ModuleBase = reinterpret_cast<uintptr_t>(GetModuleHandle(nullptr));
        Globals::GameWindowHandle = Globals::FindRfgTopWindow();
        Globals::MouseGenericPollMouseVisible = Globals::FindPattern("rfg.exe", "\x84\xD2\x74\x08\x38\x00\x00\x00\x00\x00\x75\x02", "xxxxx?????xx");
        Globals::CenterMouseCursorCall = Globals::FindPattern("rfg.exe", "\xE8\x00\x00\x00\x00\x89\x46\x4C\x89\x56\x50", "x????xxxxxx");
        Globals::Program = this;
        Globals::Gui = &this->Gui;
        Globals::Scripts = &this->Scripts;
        Globals::Camera = &this->Camera;

        Camera.Initialize(Globals::DefaultFreeCameraSpeed, 5.0);
        Functions.Initialize();
        Scripts.Initialize();

        //Creates and enables all function hooks.
        CreateHooks();

        Globals::OriginalWndProc = reinterpret_cast<WNDPROC>(SetWindowLongPtr(Globals::GameWindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(WndProc)));
        Logger::Log("Custom WndProc set.", LogInfo);

        /*Waits for ImGui init to complete before continuing to GuiSystem (The overlay) init.*/
        while (!Globals::ImGuiInitialized) //ImGui Initialization occurs in KeenGraphicsBeginFrameHook in Hooks.cpp
        {
            Sleep(100);
        }

        Gui.Initialize(); //Todo: Change gui so it can be initialized before imgui is initialized
        Gui.SetScriptManager(&Scripts);
        Gui.FreeCamSettings->Camera = &Camera;

        //Update global lua pointers after init just to be sure. Can't hurt.
        Scripts.UpdateRfgPointers();
        Scripts.ScanScriptsFolder();

        Beep(600, 100);
        Beep(700, 100);
        Beep(900, 200);

        SetMemoryLocations();
    }
    catch (std::exception& Ex)
    {
        std::string MessageBoxString = R"(Exception detected during script loader initialization! Please provide this and a zip file with your logs folder (./RFGR Script Loader/Logs/) to the maintainer.)";
        MessageBoxString += Ex.what();
        Logger::Log(MessageBoxString, LogFatalError, true, true);
        MessageBoxA(Globals::FindRfgTopWindow(), MessageBoxString.c_str(), "Script loader failed to initialize!", MB_OK);
    }
}

void Application::OpenDefaultLogs()
{
    try
    {
        Logger::Log("RFGR script loader successfully activated.", LogInfo, true);
        Logger::CloseLogFile("Load Log.txt");

        Logger::OpenLogFile("General Log.txt", LogInfo, std::ios_base::trunc);
        Logger::OpenLogFile("Error Log.txt", LogWarning | LogError | LogFatalError, std::ios_base::trunc);
        Logger::OpenLogFile("Json Log.txt", LogJson, std::ios_base::trunc);
        Logger::OpenLogFile("Lua Log.txt", LogLua, std::ios_base::trunc);
    }
    catch (std::exception& Ex)
    {
        std::string MessageBoxString = R"(Exception detected when opening the default log files. Please show this to the current script loader maintainer. It's much harder to fix any further problems which might occur without logs.)";
        MessageBoxString += Ex.what();
        Logger::Log(MessageBoxString, LogFatalError, true, true);
        MessageBoxA(Globals::FindRfgTopWindow(), MessageBoxString.c_str(), "Logger failed to open default log files!", MB_OK);
    }
}

void Application::MainLoop()
{
    const ulong UpdatesPerSecond = 1;
    const ulong MillisecondsPerUpdate = 1000 / UpdatesPerSecond;
    while (!ShouldClose()) //Todo: Change to respond to PostQuitMessage(0) in WndProc
    {
        std::chrono::steady_clock::time_point Begin = std::chrono::steady_clock::now();
        /*Note 1: The error messages in the next three if statements are BS. They really
        detect if the player has entered a multiplayer mode. I changed them to
        hopefully thwart anyone trying to disable multiplayer checks with binary
        patches. It likely won't slow down people who know what they are doing,
        but I figure it's worth a go.*/
        /*Note 2: Using a switch statement instead of an if statement since it's slightly more convincing than a bunch of states stuck to an if statement in disasm imo.*/
        const GameState RFGRState = GameseqGetState();
        switch (RFGRState)
        {
        case GS_WRECKING_CREW_MAIN_MENU:
            Logger::Log("Failed to catch exception in UI hook. Script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_WRECKING_CREW_CHARACTER_SELECT:
            Logger::Log("Null memory access attempted. Script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_WRECKING_CREW_SCOREBOARD:
            Logger::Log("Rfg deleted something important, script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_MULTIPLAYER_LIVE:
            Logger::Log("Failed to relocate rfg application struct after patch, script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_WC_INIT:
            Logger::Log("Free cam init binary patched something it shouldn't have, script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_WC_SHUTDOWN:
            Logger::Log("Dynamic allocator failure, script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        case GS_MULTIPLAYER_LIVE_FIND_SERVERS:
            Logger::Log("Failed to refresh object hashmap, script loader crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            break;
        default:
            break;
        }
        if (*Globals::InMultiplayer)
        {
            Logger::Log("Invalid graphics state in script loader overlay, crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            return;
        }
        if (Globals::MultiplayerHookTriggered)
        {
            Logger::Log("Null pointer in script loader callback system, crashing!", LogFatalError, true);
            FreeLibraryAndExitThread(Globals::ScriptLoaderModule, 0);
            return;
        }
        if (Globals::ScriptLoaderCloseRequested)
        {
            ExitKeysPressCount = 10;
        }
        std::chrono::steady_clock::time_point End = std::chrono::steady_clock::now();
        Sleep(MillisecondsPerUpdate - std::chrono::duration_cast<std::chrono::milliseconds>(End - Begin).count());
    }
}

void Application::Exit()
{
    if (Globals::OverlayActive || Gui.IsLuaConsoleActive())
    {
        SnippetManager::RestoreSnippet("MouseGenericPollMouseVisible", true);
        SnippetManager::RestoreSnippet("CenterMouseCursorCall", true);
    }
    SetWindowLongPtr(Globals::GameWindowHandle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(Globals::OriginalWndProc));
    Camera.DeactivateFreeCamera(true);

    HideHud(false);
    HideFog(false);

    Hooks.DisableAllHooks();

#if DebugDrawTestEnabled
    dd::shutdown();
#endif

    ImGui_ImplDX11_InvalidateDeviceObjects();
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();

    //Attempt to disable invulnerability and reset player health.
    if (Globals::PlayerPtr)
    {
        Globals::PlayerPtr->Flags.invulnerable = false;
        Globals::PlayerPtr->HitPoints = 230.0f;
    }

    Beep(900, 100);
    Beep(700, 100);
    Beep(600, 200);

    CloseConsole();

    Logger::Log("RFGR script loader deactivated", LogInfo, true);
    Logger::CloseAllLogFiles();
}

void Application::CreateHooks()
{
    Hooks.CreateHook("PlayerDoFrame", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x6D5A80), PlayerDoFrameHook, reinterpret_cast<LPVOID*>(&PlayerDoFrame));
    Hooks.CreateHook("ExplosionCreate", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x2EC720), ExplosionCreateHook, reinterpret_cast<LPVOID*>(&ExplosionCreate));
    Hooks.CreateHook("KeenGraphicsBeginFrame", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x86DD00), KeenGraphicsBeginFrameHook, reinterpret_cast<LPVOID*>(&KeenGraphicsBeginFrame));
    Hooks.CreateHook("KeenGraphicsResizeRenderSwapchain", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x86AB20), KeenGraphicsResizeRenderSwapchainHook, reinterpret_cast<LPVOID*>(&KeenGraphicsResizeRenderSwapchain));

    /*Start of MP Detection Hooks*/
    //Using phony names to make finding the MP hooks a bit more difficult.
    Hooks.CreateHook("FreeSubmodeDoFrame", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x516D80), HudUiMultiplayerEnterHook, reinterpret_cast<LPVOID*>(&HudUiMultiplayerEnter));
    Hooks.CreateHook("FreeSubmodeInit", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x3CC750), GameMusicMultiplayerStartHook, reinterpret_cast<LPVOID*>(&GameMusicMultiplayerStart));
    Hooks.CreateHook("SatelliteModeInit", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x4F50B0), HudUiMultiplayerProcessHook, reinterpret_cast<LPVOID*>(&HudUiMultiplayerProcess));
    Hooks.CreateHook("SatelliteModeDoFrame", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x1D0DD0), IsValidGameLinkLobbyKaikoHook, reinterpret_cast<LPVOID*>(&IsValidGameLinkLobbyKaiko));
    Hooks.CreateHook("ModeMismatchFixState", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x497740), InitMultiplayerDataItemRespawnHook, reinterpret_cast<LPVOID*>(&InitMultiplayerDataItemRespawn));
    /*End of MP Detection Hooks*/

    Hooks.CreateHook("world::do_frame", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x540AB0), world_do_frame_hook, reinterpret_cast<LPVOID*>(&world_do_frame));
    Hooks.CreateHook("rl_camera::render_begin", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x137660), rl_camera_render_begin_hook, reinterpret_cast<LPVOID*>(&rl_camera_render_begin));
    Hooks.CreateHook("hkpWorld::stepDeltaTime", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x9E1A70), hkpWorld_stepDeltaTime_hook, reinterpret_cast<LPVOID*>(&hkpWorld_stepDeltaTime));
    Hooks.CreateHook("Application::UpdateTime", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x5A880), ApplicationUpdateTimeHook, reinterpret_cast<LPVOID*>(&ApplicationUpdateTime));

    Hooks.CreateHook("LuaDoBuffer", reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x82FD20), LuaDoBufferHook, reinterpret_cast<LPVOID*>(&LuaDoBuffer));

    Hooks.CreateHook("D3D11Present", reinterpret_cast<LPVOID>(kiero::getMethodsTable()[8]), D3D11PresentHook, reinterpret_cast<LPVOID*>(&D3D11PresentObject));
}

bool Application::ShouldClose() const
{
    return ExitKeysPressCount > 5;
}

void Application::OpenConsole()
{
    if (Globals::OpenDebugConsole)
    {
        FILE* pFile = nullptr;
        Globals::PID = GetCurrentProcessId();

        if (AttachConsole(Globals::PID) == 0)
        {
            PreExistingConsole = false;
            AllocConsole();
        }
        else
        {
            PreExistingConsole = true;
        }
        freopen_s(&pFile, "CONOUT$", "r+", stdout);

        Globals::ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
        Globals::SetConsoleAttributes(Globals::ConsoleDefaultTextAttributes);
    }
}

void Application::CloseConsole()
{
    if (PreExistingConsole)
	{
		FreeConsole();
	}
}

void Application::SetMemoryLocations()
{
    Globals::InMultiplayer = reinterpret_cast<bool*>(*reinterpret_cast<DWORD*>(Globals::ModuleBase + 0x002CA210));
    if (*Globals::InMultiplayer)
    {
        MessageBoxA(Globals::FindRfgTopWindow(), "MP usage detected, shutting down!", "Multiplayer mode detected", MB_OK);
        std::cout << "MP detected. Shutting down!\n";
    }
}

/* Tries to find common installation mistakes such as placing it in the rfg root directory rather than it's own
 * folder. Returns true if errors were found. Returns false if errors were not found.
 */
bool Application::IsFolderPlacementError()
{
    std::string ExePath = Globals::GetEXEPath(false);
    if (!fs::exists(ExePath + "RFGR Script Loader/Core/")) //Detect if the core lua lib folder is missing.
    {
        if (fs::exists(ExePath + "Core/")) //Detect if the user put it in the rfg folder on accident rather than the script loader folder.
        {
            std::string ErrorString = R"(Script Loader Core folder is in the wrong directory! Make sure that it's at "Red Faction Guerrilla Re-MARS-tered\RFGR Script Loader\Core". It should not be in the same folder as rfg.exe! Shutting down script loader.)";
            Logger::Log(ErrorString, LogFatalError, true, true);
            MessageBoxA(Globals::FindRfgTopWindow(), ErrorString.c_str(), "Error! Core folder in wrong root directory!", MB_OK);
        }
        else
        {
            std::string ErrorString = R"(Script Loader Core folder not detected! Make sure that it's at "\Red Faction Guerrilla Re-MARS-tered\RFGR Script Loader\Core". Double check the installation guide for an image of how it should look when installed properly. Shutting down script loader.)";
            Logger::Log(ErrorString, LogFatalError, true, true);
            MessageBoxA(Globals::FindRfgTopWindow(), ErrorString.c_str(), "Error! Core folder not found!", MB_OK);
        }
        return true;
    }
    if (!fs::exists(ExePath + "RFGR Script Loader/Fonts/")) //Detect if the fonts folder is missing.
    {
        if (fs::exists(ExePath + "Fonts/")) //Detect if the user put it in the rfg folder on accident rather than the script loader folder.
        {
            std::string ErrorString = R"(Script Loader Fonts folder is in the wrong directory! Make sure that it's at "\Red Faction Guerrilla Re-MARS-tered\RFGR Script Loader\Fonts". It should not be in the same folder as rfg.exe! Shutting down script loader.)";
            Logger::Log(ErrorString, LogFatalError, true, true);
            MessageBoxA(Globals::FindRfgTopWindow(), ErrorString.c_str(), "Error! Fonts folder in wrong root directory!", MB_OK);
        }
        else
        {
            std::string ErrorString = R"(Script Loader Fonts folder not detected! Make sure that it's at "\Red Faction Guerrilla Re-MARS-tered\RFGR Script Loader\Fonts". Double check the installation guide for an image of how it should look when installed properly. Shutting down script loader.)";
            Logger::Log(ErrorString, LogFatalError, true, true);
            MessageBoxA(Globals::FindRfgTopWindow(), ErrorString.c_str(), "Error! Fonts folder not found.", MB_OK);
        }
        return true;
    }
    return false;
}

bool Application::LoadDataFromConfig()
{
    std::string ExePath = Globals::GetEXEPath(false);
    Logger::Log("Started loading \"Settings.json\".", LogInfo);

    if (fs::exists(ExePath + "RFGR Script Loader/Settings/Settings.json"))
    {
        if (!JsonExceptionHandler([&]
        {
            Logger::Log("Parsing \"Settings.json\"", LogInfo);
            std::ifstream Config(ExePath + "RFGR Script Loader/Settings/Settings.json");
            Config >> Globals::MainConfig;
            Config.close();
            return true;
        }, "Settings.json", "parse", "parsing"))
        {
            return false;
        }
            Logger::Log("No parse exceptions detected.", LogInfo);
    }
    else
    {
        if (!JsonExceptionHandler([&]
        {
            Globals::CreateDirectoryIfNull(ExePath + "RFGR Script Loader/Settings/");
            Logger::Log("\"Settings.json\" not found. Creating from default values.", LogWarning);

            Globals::MainConfig["Open debug console"] = false;

            std::ofstream ConfigOutput(ExePath + "RFGR Script Loader/Settings/Settings.json");
            ConfigOutput << std::setw(4) << Globals::MainConfig << "\n";
            ConfigOutput.close();
            return true;
        }, "Settings.json", "write", "writing"))
        {
            return false;
        }
            Logger::Log("No write exceptions detected.", LogInfo);
    }

    if (!JsonExceptionHandler([&]
    {
        Globals::OpenDebugConsole = Globals::MainConfig["Open debug console"].get<bool>();
        return true;
    }, "Settings.json", "read", "reading"))
    {
        return false;
    }
    Logger::Log("No read exceptions detected.", LogInfo);

    Logger::Log("Done loading \"Settings.json\".", LogInfo);
    return true;
}