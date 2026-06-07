#include "newestpackspaginator.h"
#include "common/coreservices.h"
#include "common/logging.h"

NewestPacksPaginator::NewestPacksPaginator() : Paginator<WallpaperCache>()
{
    SlidingCacheController<WallpaperCache> *controller = this->controller();
    controller->setFetch(
        std::bind(&NewestPacksPaginator::fetch, this, std::placeholders::_1, std::placeholders::_2)
    );
}