#ifndef MAIN_H
#define MAIN_H

#include <cstdio>
#include <Windows.h>
#include <wdf.h>

//cpython arm64 built from:
//https://github.com/python/cpython
//https://docs.microsoft.com/en-us/windows/iot-core/developer-tools/python

#define PY_SSIZE_T_CLEAN
#include <Python.h>
#pragma comment(lib, "python38.lib")

//#ifdef ALLOC_PRAGMA
//#pragma alloc_text (INIT, DriverEntry)
//#endif

extern "C"
{
    DRIVER_INITIALIZE DriverEntry;
    EVT_WDF_DRIVER_DEVICE_ADD DriverDeviceAdd;
    EVT_WDF_DRIVER_UNLOAD DriverUnload;
}

typedef struct _GLOBAL
{
    WDFDRIVER Driver;
    WDFDEVICE Device;
    struct _GLOBAL() : Driver(), Device() {}
}   GLOBAL, *PGLOBAL;

#include "debug.h"

#endif//MAIN_H