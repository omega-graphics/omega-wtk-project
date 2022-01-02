===
API
===

This page features all of the frontend usable classes in OmegaWTK.
All usable classes are human-readable and easily accessible to user.
For detailed API calls or documentation on other classes, see the Doxygen.

----
Core
----

.. cpp:namespace:: OmegaWTK::Core

.. cpp:type:: OmegaGTE::GRect Rect

-----
Media
-----

A set of classes used for playing or authoring media including,images,audio, and video.

.. cpp:namespace:: OmegaWTK::Media

.. cpp:class:: AudioVideoProcessor

    A simple audio and video encoder and decoder for H264 and H265. (HEVC)

**Playback**

.. cpp:class:: PlaybackDispatchQueue

    A isolated thread that is responsible for scheduling and dispatching playback of media.

.. cpp:class:: AudioPlaybackDevice

    A physical device for rendering audio samples from a media source.

.. cpp:class:: AudioPlaybackSession

    Context for playing audio from an input stream.

    Example Usage:

    .. code-block:: cpp

        auto audioFile = MediaInputStream::fromFile("./testFile.mp3");
        auto playbackSession = AudioPlaybackSession::Create(dispatchQueue);
        playbackSession.setSource(audioFile);
        playbackSession.setPlaybackDevice(playbackDevice);
        playbackSession.start();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        playbackSession.stop();


.. cpp:class:: VideoPlaybackSession

    Context for playing video from an input stream.

    .. code-block:: cpp

        auto videoFile = MediaInputStream::fromFile("./test.mp4");
        auto playbackSession = VideoPlaybackSession::Create(dispatchQueue);
        playbackSession.setSource(videoFile);
        playbackSession.setAudioPlaybackDevice(audioPlaybackDevice);
        playbackSession.setVideoFrameSink(videoView);
        playbackSession.start();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        playbackSession.stop();

**Capture**

.. cpp:class:: AudioCaptureSession

    A context for recording/previewing audio input from a AudioCaptureDevice.

.. cpp:class:: AudioCaptureDevice

    A physical device for capturing audio input such as a usb microphone or a builtin camera mic.

.. cpp:class:: VideoCaptureSession

    A context for recording/previewing video input from a VideoDevice and an AudioCaptureDevice.

    Example Usage:

    .. code-block:: cpp

        auto session = captureDevice->createCaptureSession(audioCaptureDevice);
        session->setPreviewFrameSink(videoView);
        session->setPreviewAudioOutput(audioPlaybackDevice);

.. cpp:class:: VideoDevice

    A physical device for capturing video input such as a usb webcam.


-----------
Composition
-----------

All of the necessary drawing and animation capabilities.

.. cpp:namespace:: OmegaWTK::Composition

**Drawing**

.. cpp:class:: Color

    A four component color with dynamic channel sizes ranging from 8 to 32 bits per channel. (32 to 128 bits total).

.. cpp:class:: Brush

    Represents a color or gradient that can be drawn on any object in a Canvas. ()

.. cpp:class:: Layer

    A rectangular 2D surface that can be drawn to by a Canvas.
    Layers can be resized at any time but if animation of resizing is wanted then it must be performed during a Composition session.

.. cpp:class:: Canvas

    A class for drawing to a Layer.
    Only use the `draw` methods during a Composition session issued by the parent View.

    Example Usage:

    .. code-block:: cpp

        startCompositionSession()
        auto layer = rootView->getTreeLimb()->getRootLayer();
        auto canvas = rootView->makeCanvas(layer);
        canvas.drawRect(Core::Rect {{0,0},400,400});
        canvas.sendFrame();
        endCompositionSession()

**Animation**

.. cpp:class:: AnimationCurve

    Defines a generic scalable mathematical curve.
    This object defines the scalable shape of the curve rather than absolute x,y data.

.. cpp:class:: AnimationTimeline

    A keyframe based timeline with a time range from 0.0 to 1.0.

.. cpp:class:: LayerAnimator

    An interface for animating drawing and, or effects applied to a Layer.

.. cpp:class:: ViewAnimator

    An interface for animating a re-size or other properties of a View.

-----
Views
-----

Views are the foundation of every cross-platform application.
They are responsible for displaying any visual data, and visual interaction with the application's user (through keyboard/mouse/gesture input).
All views can be created through the createView methods in the Widget class.

There are 5 standard types of Views that OmegaWTK has.

.. cpp:type:: View CanvasView

The CanvasView is the default view type.
It allows creation of sublayers and drawing to all layers through Canvases hence the `CanvasView`.

**NOTE:**

    Before performing any composition commands on this object or any related child objects, invoke *startCompositonSession.*
    When finished, invoke *endCompositionSession.*

.. cpp:class:: TextView : public View

Renders text in a rectangle.
The string that is rendered to text is encoded in 32 bit unicode characters.
In addition to rendering text, this View also can push/pop individual characters from the rendered text (A simple text editor.)

.. cpp:class:: ScrollView : public View

Encapsulates a View which is clipped to the bounds of this View.
By default, this View is rendered with the target platform's native scroll bars but they can be customized if wanted.

.. cpp:class:: VideoView : public View, public Media::FrameSink

Displays the frame data given through the FrameSink interface. Used to show live video capture preview or realtime video playback from a file.

.. cpp:class:: SVGView : public View

Displays a static or dynamic SVG graphic. (The endpoint of an SVGSession).


-------
Widgets
-------
Widgets are modular UI components consisting of one or a hierarchy of Views.
This section will cover how to implement a Widget class.

First, declare a class with as a subclass of the Widget class.
NOTE:

One Constructor is REQUIRED and is expected to conform to super constructor's format.

Two other meth

.. code-block:: cpp

    class MyWidget : public OmegaWTK::Widget {

    };

