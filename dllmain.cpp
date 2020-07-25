﻿#include "framework.h"
#include "headers/runtime/RuntimeHandler.h"

/**
 *
 *   ---------------- o
 * | How it's works ? |
 * o ----------------
 *
 *                           |---------------|
 *                    +++++> |  Relocations  | ======+
 * |---------|        |      |---------------|       |        |-------------------------------------------|         |---------------|       |---------------------------|
 * | Segment | =======+                              -======> | Reconstruct hotpoints with new base addr  | ======> | OWN FUNCTIONS | ====> | Call Original entry point |
 * |---------|        |      |---------------|       |        |-------------------------------------------|         |---------------|       |---------------------------|
 *                    +++++> |    Imports    | ======+
 *                           |---------------|
 *
 */

//Scary but necessary.
DWORD Segment::UnsafeAllocatedPointer = 0x0;
DWORD Segment::UnsafeLibraryPointer = 0x0;
SegmentFramework::oVirtualFunctionCaller SegmentFramework::OriginalVirtualCaller = 0x0;

BOOL APIENTRY DllMain (HMODULE module, DWORD callReason, LPVOID lpReserved) {

    //ALL DOCS INSIDE FUNCTIONS, AND HEADERS.

    if (callReason == DLL_PROCESS_ATTACH) {

        Segment segment;
        Logger logger (Logger::LoggerType::CONSOLE);
        RuntimeHandler runtime (segment);

        PanicUtils::SetImportant (&Segment::UnsafeLibraryPointer, reinterpret_cast<DWORD> (module));

        //Logo. 
      
        logger.Info ("deli's onecrack");


        logger.Space (2);

        logger.Info ("Loading up deli's onecrack.. ");

        logger.Space ();

        logger.Info ("| [~] Performing basic checks...");

        //Do not even ask about win server :D
        if (Utils::IsWinXPOrLater () || Utils::IsWinServBuild ()) {
           logger.Info ("| [-] I cannot run OTC on the current version of Windows.");
           logger.Info ("| [~] Please upgrade to 7 or 8, or 10. (PC build)");
           return FALSE;
        }

        logger.Info ("| [+] Your windows version meets the requirements for running OTC.");
        
        //Pretty common issue.
        if (!Utils::IsRedistPackagePresent ()) {
            logger.Info ("| [-] Could not find the msvc redist package installed on your system.");
            logger.Info ("| [~] You can download and install the redist build from here: https://www.microsoft.com/en-us/download/confirmation.aspx?id=52685.");
            return FALSE;
        }

        logger.Info ("| [+] Found the required msvc redist build on your system.");
        logger.Info ("| [+] All checks passed.");

        logger.Space ();

        logger.Info ("| [~] Extracting segment to memory....");

        runtime.ExtractSegment ();
        
        logger.Info ("| [~] Reconstructing hot-points...");

        runtime.ReconstructHotPoints ();
        
        logger.Info ("| [+] Wait until the framework complete routine work...");

        logger.Info ("| [~] Filling the dependency table... (~7-15 sec)");

        segment.GetFramework().CreateDependencyTable ();

        logger.Info ("| [~] Updating netvars...");

        segment.GetFramework().UpdateNetVars ();

        logger.Info ("| [~] Creating hook for internal function...");

        segment.GetFramework().CreateHook ();

        logger.Info ("| [~] Updating watermark...");

        //TODO: Waiting your beatiful name.
        //README: !!getenv it's a function!!. Please read docs before edit and ask questions.
        segment.GetFramework().UpdateWatermark (getenv ("USERNAME"), "deli's onecrack");
        segment.GetFramework().UpdateMenuWatermark ("deli's onecrack ");

        logger.Info ("| [~] Invoking OEP...");
       
        //Make segment alive.
        runtime.InvokeOEP ();

        //Hide menu for better log look.
        segment.GetFramework().SetMenuStatus (false);

        logger.Info ("| [+] deli's onecrack initialized.");

        logger.Space ();
       
      
        logger.Info ("Credits to https://www.github.com/0x000cb for the original build :)");

        logger.Space (2);

    }

    return TRUE;
}