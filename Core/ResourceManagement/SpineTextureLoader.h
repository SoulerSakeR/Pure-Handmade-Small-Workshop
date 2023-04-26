#pragma once
#include <spine/TextureLoader.h>
#include <spine/Atlas.h>

using namespace spine;

class SpineTextureLoader :public TextureLoader
{
public:
    SpineTextureLoader() { }

    virtual ~SpineTextureLoader() { }

    // Called when the atlas loads the texture of a page.
    virtual void load(AtlasPage& page, const String& path) {
        Texture* texture = engine_loadTexture(path);

        // if texture loading failed, we simply return.
        if (!texture) return;

        // store the Texture on the rendererObject so we can
        // retrieve it later for rendering.
        page.setRendererObject(texture);

        // store the texture width and height on the spAtlasPage
        // so spine-c can calculate texture coordinates for
        // rendering.
        page.setWidth(texture->width)
            page.setHeight(texture->height);
    }

    // Called when the atlas is disposed and itself disposes its atlas pages.
    virtual void unload(void* texture) {
        // the texture parameter is the texture we stored in the page via page->setRendererObject()
        engine_disposeTexture(texture);
    }
};

