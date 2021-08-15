#include "omegaWTK/Native/NativeNote.h"

#include <NotificationActivationCallback.h>
#include <windows.ui.notifications.h>
#include <wrl.h>
#include <appmodel.h>

#pragma comment(lib,"runtimeobject.lib")

using namespace ABI;
using namespace Microsoft::WRL;

namespace OmegaWTK::Native {

    class DECLSPEC_UUID("7dd94a99-4f0f-455d-9347-7a42276a2c5d") WinToastActivator final : public RuntimeClass<RuntimeClassFlags<ClassicCom>,INotificationActivationCallback>  {
        HRESULT STDMETHODCALLTYPE Activate(
        _In_ LPCWSTR appUserModelId,
        _In_ LPCWSTR invokedArgs,
        _In_reads_(dataCount) const NOTIFICATION_USER_INPUT_DATA* data,
        ULONG dataCount) override
        {

        };
    };

    CoCreatableClass(WinToastActivator);

    class  WinNotificationCenter 
    : public NativeNoteCenter {
    public:
        WinNotificationCenter(){
           
        }
    };

    NNCP make_native_note_center(){
        return new WinNotificationCenter();
    };




}