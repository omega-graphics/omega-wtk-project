#include "omegaWTK/Native/NativeTheme.h"

#include <windows.h>
#include <Uxtheme.h>

namespace OmegaWTK::Native {
    ThemeDesc queryCurrentTheme(){
        ThemeDesc d {};
      
        HTHEME theme = GetWindowTheme(GetForegroundWindow());
        // GetThemeColor(theme, int iPartId, int iStateId, int iPropId, COLORREF *pColor)
        return d;
    }
}