#include "omegaWTK/Native/NativeItem.h"

#include <gtk/gtk.h>
#include <gdk/gdkx.h>

#ifndef OMEGAWTK_NATIVE_GTK_GTKITEM_H
#define OMEGAWTK_NATIVE_GTK_GTKITEM_H

namespace OmegaWTK::Native::GTK {
    class GTKItem : public NativeItem {

        GdkWindow *window;
        GdkWindowClass * wnd_class;

        OmegaCommon::Vector<SharedHandle<GTKItem>> childWindows;
        Core::Rect rect;
        public:
        void enable() override;
        void disable() override;
        void resize(Core::Rect &newRect) override;
        void * getBinding() override;
        void addChildNativeItem(SharedHandle<NativeItem> nativeItem) override;
        void removeChildNativeItem(SharedHandle<NativeItem> nativeItem) override;
        void setClippedView(SharedHandle<NativeItem> clippedView) override;
        void toggleHorizontalScrollBar(bool &state) override;
        void toggleVerticalScrollBar(bool &state) override;
        GTKItem(Core::Rect rect,Native::ItemType type,NativeItemPtr parent);
        Display * getDisplay();
        Window getX11Window();
    };
};

#endif
