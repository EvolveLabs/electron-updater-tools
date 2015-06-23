/*
 * When this file is linked to a DLL, it sets up a delay-load hook that
 * intervenes when the DLL is trying to load 'node.exe' or 'iojs.exe'
 * dynamically. Instead of trying to locate the .exe file it'll just return
 * a handle to the process image.
 *
 * This allows compiled addons to work when node.exe or iojs.exe is renamed.
 */

#ifdef _MSC_VER
 
// Add "#include <delay_load_hook.h>" to your addon
// Then this to your modules bindings.gyp file:
// 'conditions': [
// ['OS=="win"', {
//   'libraries': [
//     '-lkernel32.lib',
//     '-luser32.lib',
//   ],
//   'msvs_settings': {
//     'VCLinkerTool': {
//       'DelayLoadDLLs': ['node.dll', 'iojs.exe', 'node.exe'],
//       'AdditionalOptions': ['/ignore:4199' ],
//     }
//   }
// }],
// ]
// 
// See this issue for more context:
// https://github.com/iojs/io.js/issues/751

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <delayimp.h>
#include <string.h>

static FARPROC WINAPI load_exe_hook(unsigned int event, DelayLoadInfo* info)
{
  if (event != dliNotePreLoadLibrary)
    return NULL;

  if (_stricmp(info->szDll, "iojs.exe") != 0 &&
      _stricmp(info->szDll, "node.exe") != 0 &&
      _stricmp(info->szDll, "node.dll") != 0)
  	return NULL;

  HMODULE m = GetModuleHandle(info->szDll);
  if (m == NULL) m = GetModuleHandle(NULL);
  return (FARPROC) m;
}

PfnDliHook __pfnDliNotifyHook2 = load_exe_hook;

#endif