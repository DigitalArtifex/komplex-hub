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
            <img src="https://git.digitalartifex.dev/digitalartifex/komplex-hub/raw/branch/main/images/screenshots/screenshot_05_live.png" />
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
$ git clone --recursive gitea@git.digitalartifex.dev:digitalartifex/komplex-hub.git
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

### API Status

| Feature |  Incomplete  | Started  | Complete |
|-------|-------|-------|-------|
| Newest Packs | - | - | ✅ |
| Featured Packs | - | ✔ | - |
| Search Packs | - | - | ✅ |
| Submit Packs | |❌| |-| |-|
| Newest Cubemaps | - | - | ✅ |
| Featured Cubemaps | - | ✔ | - |
| Search Cubemaps | - | - | ✅ |
| Submit Cubemaps | |❌| |-| |-|
| Newest Filter Shaders | - | - | ✅ |
| Featured Filter Shaders | - | ✔ | - |
| Search Filter Shaders | - | - | ✅ |
| Submit Filter Shaders | |❌| |-| |-|
| Pack Thumbnails | - | - | ✅ |
| Featured Images | - | - | ✅ |
| Search Images | - | - | ✅ |
| Popular Videos | - | - | ✅ |
| Search Videos | - | - | ✅ |
| User Accounts | - | ✔ | - |
| User Authentication | - | ✔ | - |
| User Configuration | |❌| |-| |-|
| User Media | |❌| |-| |-|
| Author Profiles | |❌| |-| |-|

### Media Hub Status

| Feature |  Incomplete  | Started  | Complete |
|-------|-------|-------|-------|
| UI Design | - | - | ✅ |
| Spotlight/Homepage | - | ✔ | - |
| - Newest Packs | - | - | ✅ |
| - Featured Packs | - | ✔ | - |
| - Featured Images | - | - | ✅ |
| - Popular Videos | - | - | ✅ |
| - View More | - | - | ✅ |
| - Author Profile | |❌| |-| |-|
| Media Downloader | | - | ✔ | - |
| - Packs | |❌| |-| |-|
| - Videos | |❌| |-| |-|
| - Images | - | - | ✅ |
| Detailed Views | - | - | ✅ |
| - Packs | - | - | ✅ |
| - Videos | - | - | ✅ |
| - Images | - | - | ✅ |
| Installed Media Manager | - | ✔ | - |
| Settings Manager | - | - | ✅ |
| User Accounts | |❌| |-| |-|
| - User Login | |❌| |-| |-|
| - User Profile | |❌| |-| |-|
| About Page | - | - | ✅ |

### Pack Builder

| Feature |  Incomplete  | Started  | Complete |
|-------|-------|-------|-------|
| All Design & Features | ❌ | - | - |

### Site

| Feature |  Incomplete  | Started  | Complete |
|-------|-------|-------|-------|
| Site Base | - | ✔ | - |
| Accounts | - | ✔ | - |
| Gallery | | ❌ | - | - |
| Payment Gateway | | ❌ | - | - |
| - Artist Payout | | ❌ | - | - |
| - User Media Purchase | | ❌ | - | - |

This app is currently under development and is currently alpha. The API is now being hosted on https://komplex.dev.

I have begun adding download functionality, however it is not yet complete.
"Featured" Live wallpapers is currently a duplicated of "Newest". The featured endpoint will take manual curation, so this will take some time.
BETA should be here within a few weeks.
