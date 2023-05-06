#pragma once
#include <spine/TextureLoader.h>
#include <spine/Atlas.h>
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/ResourceManagement/Texture2D.h"

using namespace spine;

class SpineTextureLoader :public TextureLoader
{
public:
    
    SpineTextureLoader() { }

    static SpineTextureLoader* get_instance() {
        if (instace_ == nullptr) {
			instace_ = new SpineTextureLoader();
		}
		return instace_;
	}
    virtual ~SpineTextureLoader() { }

    // Called when the atlas loads the texture of a page.
    virtual void load(AtlasPage& page, const String& path) {
        std::string path_str(path.buffer(),path.length());
        auto texture = Texture2D::loadFromImgPath(path_str);
        texture->set_texture(texture->get_img_path(), false, false);
        // if texture loading failed, we simply return.
        if (texture==nullptr) 
            return;

        // store the Texture on the rendererObject so we can
        // retrieve it later for rendering.
        page.setRendererObject(texture);

        // store the texture width and height on the spAtlasPage
        // so spine-c can calculate texture coordinates for
        // rendering.
        page.width = texture->get_width();
        page.height = texture->get_height();
    }

    // Called when the atlas is disposed and itself disposes its atlas pages.
    virtual void unload(void* texture) {
        // the texture parameter is the texture we stored in the page via page->setRendererObject()
        auto ptr = static_cast<Texture2D*>(texture);
        delete ptr;
    }

private:
    static SpineTextureLoader* instace_;
};

SpineTextureLoader* SpineTextureLoader::instace_ = nullptr;

#include <spine/Extension.h>

spine::SpineExtension* spine::getDefaultExtension() {
    return new spine::DefaultSpineExtension();
}