#include "main.h"

static GLOBAL Global;

//TODO: Use Device Memory Access Methods e.g. READ_REGISTER_ULONG() and WRITE_REGISTER_ULONG()
//      and give a stable interface to python so that it can control GPIO based on Python HAT Drivers

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    debug("[CALL]: %s START", "DriverEntry");
    NTSTATUS status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG config;

    WDF_DRIVER_CONFIG_INIT(&config, DriverDeviceAdd);
    config.EvtDriverUnload = DriverUnload;
    status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, &Global.Driver);
    if (NT_ERROR(status)) { return status; }
    
    debug("[CALL]: %s END", "DriverEntry");
    return STATUS_SUCCESS;
}

NTSTATUS DriverDeviceAdd(WDFDRIVER Driver, PWDFDEVICE_INIT DeviceInit)
{
    debug("[CALL]: %s START", "DriverDeviceAdd"); 
    NTSTATUS status = STATUS_SUCCESS;
    CHAR narrowString[DEBUG_BUFFSIZE] = "";
    WCHAR wideString[DEBUG_BUFFSIZE] = L"";
    int retval = 0;
    
    Global.Driver = Driver;
    status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &Global.Device);
    if (!NT_SUCCESS(status)) { return status; }
    
    //Init Python Interpreter Here for HAT Communication?
    debug("[CKPT]: %d", 1);
    Py_SetProgramName(L"Python C++ Interpreter Driver");
    GetCurrentDirectoryW(DEBUG_BUFFSIZE, wideString);
    wcscat_s(wideString, L"\\drivers\\UMDF\\Python");
    debug("[INFO]: GetCurrentDirectoryW() = %ls", wideString);
    Py_SetPythonHome(wideString);
    debug("[CKPT]: %d", 2);
    Py_Initialize();
    debug("[CKPT]: %d", 3);
    retval = Py_IsInitialized();
    if (0 == retval) { debug("[WARN]: %s", "Python Module Setup Failed"); return STATUS_DEVICE_NOT_READY; }
    debug("[CKPT]: %d", 4);
    debug("[INFO]: Version: %hs", Py_GetVersion());
    debug("[INFO]: Name: %ls", Py_GetProgramName());
    debug("[INFO]: Home: %ls", Py_GetPythonHome());
    debug("[INFO]: Path: %ls", Py_GetPath());
    debug("[CKPT]: %d", 5);
    PyObject* globals = Py_BuildValue("{}");
    PyObject* locals = Py_BuildValue("{}");
    debug("[CKPT]: %d", 6);
    /*
    PyObject* result = PyRun_StringFlags( //import sys and sys.path, sys.version etc. next
        "import os" "\n"
        //"import start" "\n"
        , Py_file_input, globals, locals, NULL);
    */
    
    PyObject* result = PyRun_StringFlags( //import sys and sys.path, sys.version etc. next
        "import os" "\n"
        "with open('UDFPython.txt', 'w') as file:" "\n"
        "    file.seek(0)" "\n"
        "    file.write(os.getcwd())" "\n"
        "    file.truncate()" "\n"
        "\n"
        //"import start" "\n"
        , Py_file_input, globals, locals, NULL);

    UNREFERENCED_PARAMETER(result);
    debug("[CKPT]: %d", 7);
    if (PyErr_Occurred())
    {
        debug("[CKPT]: %s", "7.1");
        PyObject* type = Py_BuildValue("{}");
        PyObject* value = Py_BuildValue("{}");
        PyObject* traceback = Py_BuildValue("{}");
        debug("[CKPT]: %s", "7.2");
        PyErr_Fetch(&type, &value, &traceback);
        debug("[CKPT]: %s", "7.3");
        PyObject* typeStr = PyObject_Str(type); if (NULL == typeStr) { typeStr = Py_BuildValue("{}"); }
        PyObject* valueStr = PyObject_Str(value); if (NULL == valueStr) { valueStr = Py_BuildValue("{}"); }
        PyObject* tracebackStr = PyObject_Str(traceback); if (NULL == tracebackStr) { tracebackStr = Py_BuildValue("{}"); }
        debug("[CKPT]: %s", "7.4");
        debug("[WARN]: %s | type: %s | value: %s | traceback: %s", "Python_Run() Failed",
            PyUnicode_AsUTF8(typeStr), PyUnicode_AsUTF8(valueStr), PyUnicode_AsUTF8(tracebackStr));
        debug("[CKPT]: %s", "7.5");
        PyErr_Clear();
        debug("[CKPT]: %s", "7.6");
    }
    else { debug("[INFO]: %s", "Python_Run() Success"); }
    debug("[CKPT]: %d", 8);
    debug("[CALL]: %s END", "DriverDeviceAdd");
    return STATUS_SUCCESS;
}

void DriverUnload(WDFDRIVER Driver)
{
    debug("[CALL]: %s START", "DriverUnload");
    UNREFERENCED_PARAMETER(Driver);
    Py_Finalize();
    debug("[CALL]: %s END", "DriverUnload");
}