VisionRGBApp
==================

Datapath Vision series capture app.

Licence: Public domain

This project aims to be a loose reimplementation of the native Vision app bundled with the Datapath drivers. Notable differences include a Direct3D backend, DXVA2 image processing (GPU accelerated colour space conversion, motion adaptive deinterlacing), and a preset system similar to that of [leikareipa/VCS](https://github.com/leikareipa/vcs). It intends to replace the common use case of window capturing the Vision window in OBS with game capturing the VisionRGBApp window in OBS, where due to the nature of shared texture game capture, the video buffer never touches system memory, greatly reducing latency and system load, comparable to having a native OBS plugin, but with the flexibility of an application window and configuration independent of OBS.

This initial commit is nothing more than a slightly modified version of the DirectX9 sample from the Win32++ framework repository. I had written a proof of concept in native Win32 code, however I wanted to have access to common quality-of-life features like a rebar/toolbar and resource-based dialogue forms without having to write pages upon pages of Win32 boilerplate code, so I settled on rewriting the application using the Win32++ framework. As an added bonus this gives me the opportunity to host the project on GitHub right from the beginning.