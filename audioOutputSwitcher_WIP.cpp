#include <initguid.h>  // Get rid of linker errors.
#include <Windows.h>
#include <Mmdeviceapi.h>
#include <iostream>
#include <string>

DEFINE_PROPERTYKEY(PKEY_Device_FriendlyName, 0xa45c254e, 0xdf1c, 0x4efd, 0x80, 0x20, 0x67, 0xd1, 0x46, 0xa8, 0x50, 0xe0, 14);

HRESULT GetDeviceFriendlyName(IMMDevice *pDevice, LPWSTR *ppFriendlyName) {
    IPropertyStore *pPropertyStore = nullptr;
    HRESULT hr = pDevice->OpenPropertyStore(STGM_READ, &pPropertyStore);
    if (SUCCEEDED(hr)) {
        PROPVARIANT pv;
        PropVariantInit(&pv);

        hr = pPropertyStore->GetValue(PKEY_Device_FriendlyName, &pv);
        if (SUCCEEDED(hr)) {
            *ppFriendlyName = pv.pwszVal;
        }

        pPropertyStore->Release();
    }
    return hr;
}

int main() {
    HRESULT hr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(hr)) {
        IMMDeviceEnumerator* pEnumerator = nullptr;
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL,
    __uuidof(IMMDeviceEnumerator), (LPVOID*)&pEnumerator);

        if (SUCCEEDED(hr)) {
            IMMDeviceCollection* pDeviceCollection = nullptr;
            hr = pEnumerator->EnumAudioEndpoints(eRender, DEVICE_STATE_ACTIVE,
   &pDeviceCollection);

            if (SUCCEEDED(hr)) {
                UINT deviceCount = 0;
                hr = pDeviceCollection->GetCount(&deviceCount);

                if (SUCCEEDED(hr)) {
                    for (UINT i = 0; i < deviceCount; ++i) {
                        IMMDevice* pDevice = nullptr;
                        hr = pDeviceCollection->Item(i, &pDevice);

                        if (SUCCEEDED(hr)) {
                            // Get device properties
                            LPWSTR deviceId = nullptr;
                            hr = pDevice->GetId(&deviceId);
                            if (SUCCEEDED(hr)) {
                                // Get and print friendly name
                                LPWSTR friendlyName = nullptr;
                                hr = GetDeviceFriendlyName(pDevice, &friendlyName);

                                if (SUCCEEDED(hr)) {
                                    /* color spaghetti */
                                    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
                                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                                    std::wcout << L"NAME: ";
                                    SetConsoleTextAttribute(hConsole, 7);
                                    std::wcout << friendlyName << std::endl;
                                    SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
                                    std::wcout <<"ID: ";
                                    SetConsoleTextAttribute(hConsole, 7);
                                    std::wcout << deviceId << std::wstring(2, '\n');
                                    CoTaskMemFree(friendlyName);
                                }

                                CoTaskMemFree(deviceId);
                            }

                            pDevice->Release();
                        }
                    }
                }

                pDeviceCollection->Release();
            }

            pEnumerator->Release();
        }

        CoUninitialize();
    }

    return 0;
}
