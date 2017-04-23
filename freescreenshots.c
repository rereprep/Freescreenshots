#include <psp2/kernel/modulemgr.h>
#include <psp2/sysmodule.h>
#include <psp2/screenshot.h>
#include <taihen.h>

static tai_hook_ref_t load_hook;
static tai_hook_ref_t unload_hook;
static tai_hook_ref_t ss_overlay_hook;
static SceUID ss_overlay_uid;
static tai_hook_ref_t ss_disable_hook;
static SceUID ss_disable_uid;

// hook to never disable screenshots
int hook_ss_disable(void) {
  TAI_CONTINUE(int, ss_disable_hook); // so others get a chance to hook
  int ret = sceScreenShotEnable(); // we always re-enable ss
  return ret;
}

// hook to never watermark screenshots
int hook_ss_overlay(const char *filepath, int offsetX, int offsetY) {
   int ret = TAI_CONTINUE(int, ss_overlay_hook, NULL, offsetX, offsetY); // so others get a chance to hook
   return ret;
}

// hook load module
int hook_sysmodule_load(uint16_t id) {
  int ret;
  ret = TAI_CONTINUE(int, load_hook, id);
  if (ret >= 0) { // load successful
    switch (id) {
      case SCE_SYSMODULE_SCREEN_SHOT:
        ss_overlay_uid = 
         taiHookFunctionExport(&ss_overlay_hook, // Output a reference
                        "SceScreenShot",         // Name of module being hooked
                        0xF26FC97D,               // NID specifying SceScreenShot
                        0x7061665B,               // NID specifying sceScreenShotSetOverlayImage
                        hook_ss_overlay);         // Name of the hook function
		ss_disable_uid = 
          taiHookFunctionExport(&ss_disable_hook, // Output a reference
                        "SceScreenShot",          // Name of module being hooked
                        0xF26FC97D,               // NID specifying SceScreenShot
                        0x50AE9FF9,               // NID specifying sceScreenShotDisable
                        hook_ss_disable);    
    }
  }
  return ret;
}

// hook unload module
int hook_sysmodule_unload(uint16_t id) {
  int ret;
  ret = TAI_CONTINUE(int, unload_hook, id);
  if (ret >= 0) { // unload successful
    switch (id) {
      case SCE_SYSMODULE_SCREEN_SHOT:
        if (ss_overlay_uid >= 0) {
          taiHookRelease(ss_overlay_uid, ss_overlay_hook);
          ss_overlay_uid = -1;
        }
	if (ss_disable_uid >= 0) {
          taiHookRelease(ss_disable_uid, ss_disable_hook);
          ss_disable_uid = -1;
	} 
    }
  }
  return ret;
}

// our own plugin entry
int module_start(SceSize argc, const void *args) {
  ss_disable_uid = -1;
  ss_overlay_uid = -1;
  taiHookFunctionImport(&load_hook,             // Output a reference
                        TAI_MAIN_MODULE,              // Name of module being hooked
                        0x03FCF19D,             // NID specifying SceSysmodule
                        0x79A0160A,             // NID specifying sceSysmoduleLoadModule
                        hook_sysmodule_load);   // Name of the hook function
  taiHookFunctionImport(&unload_hook,           // Output a reference
                        TAI_MAIN_MODULE,              // Name of module being hooked
                        0x03FCF19D,             // NID specifying SceSysmodule
                        0x31D87805,             // NID specifying sceSysmoduleUnloadModule
                        hook_sysmodule_unload); // Name of the hook function
  return SCE_KERNEL_START_SUCCESS;
}

