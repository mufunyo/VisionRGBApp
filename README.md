VisionRGBApp
==================

Datapath Vision series capture app. [![Public domain](https://img.shields.io/badge/license-public%20domain-brightgreen)](https://github.com/mufunyo/VisionRGBApp/blob/trunk/LICENSE) [![Works on My Machine](https://img.shields.io/badge/works%20on-my%20machine-yellow)](https://blog.codinghorror.com/the-works-on-my-machine-certification-program/)

This project aims to be a loose reimplementation of the native Vision app bundled with the Datapath drivers. Notable differences include a Direct3D backend, DXVA2 image processing (GPU accelerated colour space conversion, motion adaptive deinterlacing), and a preset system similar to that of [leikareipa/VCS](https://github.com/leikareipa/vcs). It intends to replace the common use case of window capturing the Vision window in OBS with game capturing the VisionRGBApp window in OBS, where due to the nature of shared texture game capture, the video buffer never touches system memory, greatly reducing latency and system load, comparable to having a native OBS plugin, but with the flexibility of an application window and configuration independent of OBS.
