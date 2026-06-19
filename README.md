# Komplex Hub

> Media Hub and Settings Manager for the Komplex Wallpaper Plugin

<table width="100%">
    <tr>
        <td colspan="2">
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_01_home.png" />
        </td>
    </tr>
    <tr>
        <td>
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_02_no_results.png" />
        </td>
        <td>
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_03_images.png" />
        </td>
    </tr>
    <tr>
        <td>
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_04_videos.png" />
        </td>
        <td>
            <img src="" />
        </td>
    </tr>
    <tr>
        <td>
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_06_installed.png" />
        </td>
        <td>
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_07_settings.png" />
        </td>
    </tr>
</table>

Most of the development of Komplex and it's associated apps will now happen here, instead of Github.

## Installation

#### Requirements

- Qt 6.10+ -- May work with previous versions, but currently untested
  - Qt6 Core
  - Qt6 Gui
  - Qt6 Widgets
  - Qt6 Networking
  - Qt6 Quick
  - Qt6 Quick Effects
  - Qt6 Quick Controls
  - Qt6 Shader Tools
- CMake
- clang

#### Build
```console
$ git clone gitea@git.digitalartifex.dev:digitalartifex/komplex-hub.git
$ mkdir build
$ cmake -S ./ -B ./build/
$ cmake --build ./build
```

#### Install
```console
$ cmake --install ./build
```

## Accounts

Accounts are not an integral part of Komplex. They do not enable any "pro" benefits, they do not cost anything and will not be required to use the millions of free wallpaper, image and video offerings available.

Accounts ***will*** be required to view any content that gets flagged as ***Not Safe For Work*** and any content submitted by an artist for sale.
Content purchases will be processed through the website using Paypal as will artist payouts.

## Status

This app is currently under development and is pre-alpha. During this phase, the new API is only hosted locally and not yet available on digitalartifex.dev
