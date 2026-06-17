/*
 *  Komplex Wallpaper Engine
 *  Copyright (C) 2026 @DigitalArtifex
 *  https://digitalartifex.dev - https://github.com/DigitalArtifex
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>
 */
#ifndef KOMPLEX_GLOBAL_H
#define KOMPLEX_GLOBAL_H

#ifdef KOMPLEX_PLUGIN
#define KOMPLEX_EXPORT Q_DECL_EXPORT
#else
#define KOMPLEX_EXPORT Q_DECL_IMPORT
#endif

#define SLIDING_CACHE_WINDOW_SIZE 100
#define SLIDING_CACHE_PREFETCH_THRESHOLD 20

#define NETWORK_TIMEOUT 10000

#ifdef KOMPLEX_LOCAL_DEV
#define KOMPLEX_API_HOST                        "https://komplexapi.hlab"
#else
#define KOMPLEX_API_HOST                        "https://komplex.digitalartifex.dev"
#endif

#define KOMPLEX_API_VERSION                     "v2"

#define KOMPLEX_ENDPOINT_IMAGES_SEARCH          "images/search"
#define KOMPLEX_ENDPOINT_IMAGES_FEATURED        "featured/images"
#define KOMPLEX_ENDPOINT_IMAGES_ITEM            "images/item"

#define KOMPLEX_ENDPOINT_VIDEOS_SEARCH          "videos/search"
#define KOMPLEX_ENDPOINT_VIDEOS_FEATURED        "featured/videos"
#define KOMPLEX_ENDPOINT_VIDEOS_ITEM            "videos/item"

#define KOMPLEX_ENDPOINT_SHADERS_SEARCH         "shaders/search"
#define KOMPLEX_ENDPOINT_SHADERS_FEATURED       "shaders/featured"
#define KOMPLEX_ENDPOINT_SHADERS_ITEM           "shaders/item"
#define KOMPLEX_ENDPOINT_SHADERS_NEWEST         "shaders/newest"

#define KOMPLEX_ENDPOINT_CUBEMAPS_SEARCH        "cubemaps/search"
#define KOMPLEX_ENDPOINT_CUBEMAPS_FEATURED      "cubemaps/featured"
#define KOMPLEX_ENDPOINT_CUBEMAPS_ITEM          "cubemaps/item"
#define KOMPLEX_ENDPOINT_CUBEMAPS_NEWEST        "cubemaps/newest"

#define KOMPLEX_ENDPOINT_PACKS_SEARCH           "packs/search"
#define KOMPLEX_ENDPOINT_PACKS_FEATURED         "packs/featured"
#define KOMPLEX_ENDPOINT_PACKS_ITEM             "packs/item"
#define KOMPLEX_ENDPOINT_PACKS_NEWEST           "packs/newest"

#endif // KOMPLEX_GLOBAL_H
