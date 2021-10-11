#include "NativePrivate/gtk/GTKItem.h"

namespace OmegaWTK::Native::GTK {

//    typedef struct GTKItem_C_Class {
//        GtkWidgetClass parent_class;
//        GTKItem *cpp_binding;
//    } GTKItem_C_;
//
//
//
//    G_DEFINE_TYPE (GTKItem_C_,gtk_item, GTK_TYPE_CONTAINER)
//
//    static void gtk_item_add(GtkContainer *widget,GtkWidget *child){
//        GTKItem_C_ *klass = (GTKItem_C_ *)widget;
//    }
//
//    static void gtk_item_class_init(GTKItem_C_Class *myClass){
//        GtkContainerClass *containerClass = GTK_CONTAINER_CLASS(myClass);
//        containerClass->add = gtk_item_add;
//    }



    GTKItem::GTKItem(Core::Rect rect,Native::ItemType type,NativeItemPtr parent):rect(rect){
        GdkWindowAttr attrs;
        window = gdk_window_new(std::dynamic_pointer_cast<GTKItem>(parent)->window,&attrs,GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL);

    }

    void GTKItem::resize(Core::Rect &newRect) {
        gdk_window_move_resize(window,(gint)newRect.pos.x,(gint)newRect.pos.y,(gint)newRect.w,(gint)newRect.h);
    }

    void GTKItem::enable() {
        gdk_window_hide(window);
    }

    void GTKItem::disable() {
        gdk_window_show(window);
    }

    void *GTKItem::getBinding() {
        return reinterpret_cast<void *>(getX11Window());
    }

    void GTKItem::addChildNativeItem(NativeItemPtr nativeItem) {
        auto item = std::dynamic_pointer_cast<GTKItem>(nativeItem);
        gdk_window_reparent(item->window,window,(gint)item->rect.pos.x,(gint)item->rect.pos.y);
        childWindows.push_back(item);
    }

    void GTKItem::removeChildNativeItem(NativeItemPtr nativeItem) {
        auto item = std::dynamic_pointer_cast<GTKItem>(nativeItem);
        gdk_window_reparent(item->window,NULL,(gint)item->rect.pos.x,(gint)item->rect.pos.y);
        for(auto child_it = childWindows.begin();child_it != childWindows.end();child_it++){
            if(*child_it == item){
                childWindows.erase(child_it);
                break;
            }
        }

    }

    void GTKItem::setClippedView(SharedHandle<NativeItem> clippedView) {

    }

    Display *GTKItem::getDisplay() {
        return gdk_x11_display_get_xdisplay(gdk_window_get_display(window));
    }

    Window GTKItem::getX11Window() {
        return gdk_x11_window_get_xid(window);
    }
}

namespace OmegaWTK::Native {
    NativeItemPtr make_native_item(Core::Rect rect,Native::ItemType type,NativeItemPtr parent){
        return (NativeItemPtr)new GTK::GTKItem(rect,type,parent);
    }
}

