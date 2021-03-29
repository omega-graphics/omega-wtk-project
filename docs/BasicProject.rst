========================
How to Setup A Basic App
========================


.. code-block:: cpp

    int omegaWTKMain(OmegaWTK::AppInst *app){
        
        SharedHandle<AppWindow> window = make<AppWindow>(OmegaWTK::Rect(0,0,1000,1000));
        using namespace OmegaWTK;

        SharedHandle<UI::PushButton> button = make<UI::PushButton>(Rect(0,0,200,100));

        window->addWidget(button);

        app->windowManager->setRootWindow(window);
        app->windowManager->displayRootWindow();
        return 0;
    };

