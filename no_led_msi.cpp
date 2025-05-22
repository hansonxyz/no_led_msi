#include <windows.h>
#include <iostream>
#include <cstdint>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <Wbemidl.h>
#include <comutil.h>
#include <Shlobj.h>
#include <map>
#include <list>
#include <memory>

#pragma comment(lib, "wbemuuid.lib")
#pragma comment(lib, "comsuppw.lib")

using namespace std::string_literals;

const auto DRV_FILE_NAME = L"inpoutx64.sys";
const auto DRV_DEVICE_NAME = L"inpoutx64";
const auto DRV_SVC_NAME = L"inpoutx64";

const auto IOCTL_READ_PORT_UCHAR = CTL_CODE(0x9C40, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS);
const auto IOCTL_WRITE_PORT_UCHAR = CTL_CODE(0x9C40, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS);

const std::uint8_t RGB_BANK = 0x12;
const std::uint8_t UNKNOWN_BANK = 0x09;

struct IoctlInputBuffer {
    std::uint16_t port;
    std::uint8_t data;
};

struct IoctlOutputBuffer {
    std::uint8_t data;
};

class IsaDrv {
public:
    enum class ErrorCode {
        LoadFailed
    };

    class Exception : public std::runtime_error {
    public:
        Exception(ErrorCode ec) : std::runtime_error(""), ec(ec) {}
        ErrorCode error_code() { return ec; }
    private:
        ErrorCode ec;
    };

    IsaDrv() {
        if (!load_drv()) {
            throw Exception(ErrorCode::LoadFailed);
        }

        drv_handle = CreateFileW((L"\\\\.\\"s + DRV_DEVICE_NAME).c_str(),
            GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

        if (drv_handle == INVALID_HANDLE_VALUE) {
            throw Exception(ErrorCode::LoadFailed);
        }
    }

    ~IsaDrv() {
        CloseHandle(drv_handle);
        unload_drv();
    }

    std::uint8_t inb(std::uint8_t port) const {
        IoctlInputBuffer input_buf;
        input_buf.port = static_cast<std::uint16_t>(port);
        input_buf.data = 0;

        IoctlOutputBuffer output_buf{ 0 };

        DeviceIoControl(drv_handle, IOCTL_READ_PORT_UCHAR,
            static_cast<LPVOID>(&input_buf), sizeof(IoctlInputBuffer),
            static_cast<LPVOID>(&output_buf), sizeof(IoctlOutputBuffer), NULL, NULL);

        return output_buf.data;
    }

    void outb(std::uint8_t port, std::uint8_t data) const {
        IoctlInputBuffer input_buf{ 0 };
        input_buf.port = static_cast<std::uint16_t>(port);
        input_buf.data = data;

        DeviceIoControl(drv_handle, IOCTL_WRITE_PORT_UCHAR,
            static_cast<LPVOID>(&input_buf), sizeof(IoctlInputBuffer), NULL, 0, NULL, NULL);
    }

private:
    HANDLE drv_handle;

    DWORD install_drv() {
        // Read driver from file
        std::ifstream file("inpoutx64.sys", std::ios::binary | std::ios::ate);
        if (!file.is_open()) {
            return ERROR_FILE_NOT_FOUND;
        }

        std::streamsize size = file.tellg();
        file.seekg(0, std::ios::beg);
        std::vector<char> buffer(size);
        if (!file.read(buffer.data(), size)) {
            return ERROR_READ_FAULT;
        }
        file.close();

        // Save it to System32\Drivers\<driver_name>.sys
        PWSTR sysPath;
        HRESULT hr = SHGetKnownFolderPath(FOLDERID_System, 0, NULL, &sysPath);
        auto drv_filepath = std::wstring(sysPath).append(L"\\drivers\\"s + DRV_FILE_NAME);
        CoTaskMemFree(sysPath);

        auto outfile = std::ofstream(drv_filepath, std::ofstream::out | std::ofstream::binary | std::ofstream::trunc);
        outfile.write(buffer.data(), size);
        outfile.flush();
        outfile.close();

        // Create service for driver
        SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
        SC_HANDLE service = CreateServiceW(sc_manager, DRV_SVC_NAME, DRV_SVC_NAME,
            SERVICE_START | SERVICE_STOP, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START,
            SERVICE_ERROR_NORMAL, drv_filepath.c_str(), NULL, NULL, NULL, NULL, NULL);

        if (service == NULL) {
            DWORD err = GetLastError();
            if (err == ERROR_SERVICE_EXISTS) {
                err = ERROR_SUCCESS;
            }
            CloseServiceHandle(sc_manager);
            return err;
        } else {
            CloseServiceHandle(service);
            CloseServiceHandle(sc_manager);
            return ERROR_SUCCESS;
        }
    }

    DWORD start_drv() {
        SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
        SC_HANDLE service = OpenServiceW(sc_manager, DRV_SVC_NAME, SERVICE_START);

        if (service == NULL) {
            DWORD err = GetLastError();
            CloseServiceHandle(sc_manager);
            return err;
        }

        if (!StartService(service, 0, NULL)) {
            DWORD err = GetLastError();
            if (err == ERROR_SERVICE_ALREADY_RUNNING) {
                err = ERROR_SUCCESS;
            }
            CloseServiceHandle(service);
            CloseServiceHandle(sc_manager);
            return err;
        } else {
            CloseServiceHandle(service);
            CloseServiceHandle(sc_manager);
            return ERROR_SUCCESS;
        }
    }

    DWORD stop_drv() {
        SC_HANDLE sc_manager = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT);
        SC_HANDLE service = OpenServiceW(sc_manager, DRV_SVC_NAME, SERVICE_STOP);

        if (service == NULL) {
            DWORD err = GetLastError();
            CloseServiceHandle(sc_manager);
            return err;
        }

        SERVICE_STATUS _;
        if (!ControlService(service, SERVICE_CONTROL_STOP, &_)) {
            DWORD err = GetLastError();
            if (err == ERROR_SERVICE_NOT_ACTIVE) {
                err = ERROR_SUCCESS;
            }
            CloseServiceHandle(service);
            CloseServiceHandle(sc_manager);
            return err;
        } else {
            CloseServiceHandle(service);
            CloseServiceHandle(sc_manager);
            return ERROR_SUCCESS;
        }
    }

    bool load_drv() {
        if (install_drv() != ERROR_SUCCESS) {
            return false;
        } else {
            if (start_drv() != ERROR_SUCCESS) {
                return false;
            } else {
                return true;
            }
        }
    }

    bool unload_drv() {
        return stop_drv() == ERROR_SUCCESS;
    }
};

class Sio {
public:
    Sio() : drv(new IsaDrv) {}

    std::uint8_t read_uint8_from_bank(std::uint8_t bank, std::uint8_t index) const {
        enter_extended_function_mode();
        enter_bank(bank);
        std::uint8_t ret = read_uint8(index);
        exit_extended_function_mode();
        return ret;
    }

    void write_uint8_to_bank(std::uint8_t bank, std::uint8_t index, std::uint8_t data) const {
        enter_extended_function_mode();
        enter_bank(bank);
        write_uint8(index, data);
        exit_extended_function_mode();
    }

private:
    std::unique_ptr<IsaDrv> drv;

    void enter_extended_function_mode() const {
        drv->outb(0x4E, 0x87);
        drv->outb(0x4E, 0x87);
    }

    void exit_extended_function_mode() const {
        drv->outb(0x4E, 0xAA);
    }

    void enter_bank(std::uint8_t bank) const {
        drv->outb(0x4E, 0x07);
        drv->outb(0x4F, bank);
    }

    std::uint8_t read_uint8(std::uint8_t index) const {
        drv->outb(0x4E, index);
        return drv->inb(0x4F);
    }

    void write_uint8(std::uint8_t index, std::uint8_t data) const {
        drv->outb(0x4E, index);
        drv->outb(0x4F, data);
    }
};

std::map<std::wstring, std::wstring> wmi_query(const std::wstring& class_name, const std::list<std::wstring>& properties) {
    IWbemLocator* loc = nullptr;
    CoCreateInstance(CLSID_WbemLocator, 0, CLSCTX_INPROC_SERVER, IID_IWbemLocator, reinterpret_cast<LPVOID*>(&loc));

    IWbemServices* svc = nullptr;
    loc->ConnectServer(_bstr_t(L"ROOT\\CIMV2"), NULL, NULL, 0, NULL, 0, 0, &svc);

    CoSetProxyBlanket(svc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, NULL,
        RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE, NULL, EOAC_NONE);

    IEnumWbemClassObject* enumerator = nullptr;

    std::wostringstream query;
    query << "SELECT ";

    auto it = properties.begin();
    for (size_t i = 0; i < properties.size() - 1; ++i, ++it) {
        query << *it << ", ";
    }
    query << properties.back();
    query << " FROM " << class_name;

    svc->ExecQuery(_bstr_t("WQL"), _bstr_t(query.str().c_str()),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY, NULL, &enumerator);

    IWbemClassObject* object = nullptr;
    ULONG no_results = 0;
    enumerator->Next(WBEM_INFINITE, 1, &object, &no_results);

    std::map<std::wstring, std::wstring> results;
    for (auto p_name : properties) {
        VARIANT vt_prop;
        object->Get(p_name.c_str(), 0, &vt_prop, 0, 0);
        results.emplace(p_name, std::wstring(vt_prop.bstrVal, SysStringLen(vt_prop.bstrVal)));
    }

    object->Release();
    enumerator->Release();
    svc->Release();
    loc->Release();

    return results;
}

bool is_msi_motherboard() {
    auto info = wmi_query(L"Win32_BaseBoard", { L"Manufacturer" });
    return info[L"Manufacturer"].find(L"Micro-Star International Co., Ltd") != std::wstring::npos ||
           info[L"Manufacturer"].find(L"MSI") != std::wstring::npos;
}

void turn_off_leds() {
    Sio sio;
    
    // Read current values
    std::uint8_t val_at_e4 = sio.read_uint8_from_bank(RGB_BANK, 0xE4);
    
    // Disable LEDs by setting flashing mode bits to 001 (disabled)
    // and clearing breathing mode bit
    val_at_e4 &= 0b11110000; // Clear last 4 bits (breathing + flashing)
    val_at_e4 |= 0b1; // Set flashing mode to 001 (disabled)
    
    // Write the value back
    sio.write_uint8_to_bank(RGB_BANK, 0xE4, val_at_e4);
    
    std::wcout << L"MSI RGB LEDs have been turned off." << std::endl;
}

int main() {
    CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_SPEED_OVER_MEMORY);
    
    try {
        if (!is_msi_motherboard()) {
            std::wcout << L"Error: This is not an MSI motherboard." << std::endl;
            return 1;
        }
        
        turn_off_leds();
        return 0;
    }
    catch (const IsaDrv::Exception& e) {
        std::wcout << L"Error: Failed to load driver. Make sure you're running as administrator." << std::endl;
        return 1;
    }
    catch (const std::exception& e) {
        std::wcout << L"Error: " << e.what() << std::endl;
        return 1;
    }
    
    CoUninitialize();
}