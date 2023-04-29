#include "SpineAnimator.h"
#include "Core/ResourceManagement/ResourceMgr.h"
#include "Core/Utils/Time.h"
#include "Property.h"
#include "spine/Slot.h"
#include "spine/SlotData.h"
#include <spine/BlendMode.h>
#include <spine/RTTI.h>
#include <spine/RegionAttachment.h>
#include <spine/MeshAttachment.h>
#include <spine/Vector.h>
#include <spine/Animation.h>

using namespace spine;

unsigned short quadIndices[] = { 0, 1, 2, 2, 3, 0 };

SpineAnimator::SpineAnimator(GameObject* gameobj): IRenderable(gameobj)
{
	componentType = ComponentType::SPINE_ANIMATOR;
	properties.emplace("SpineAnimationName", new Property("SpineAnimationName", &spine_animation_name, Property::STRING, this));
    properties.emplace("Animation", new Property("Animation", &animation_index, Property::ANIMATION_COMBOBOX, this));
}

bool SpineAnimator::set_spine_animation_name(const std::string& name)
{
	auto animationdata = ResourceMgr::get_instance().loadFromName<SpineAnimationData>(name);
	if (animationdata != nullptr)
	{
        reset();
		spine_animation_name = name;
		skeleton = new spine::Skeleton(animationdata->get_skeleton_data());
		animation_state = new spine::AnimationState(animationdata->get_animation_state_data());
        skeleton->updateWorldTransform();
        onPropertyChanged(properties["SpineAnimationName"]);
        onPropertyChanged(properties["Animation"]);
		return true;
	}
	return false;
}

std::vector<std::string> SpineAnimator::getAllAnimations()
{
    std::vector<std::string> animations;
    if (skeleton != nullptr)
    {
        spine::Vector<spine::Animation*>& vec = skeleton->getData()->getAnimations();
        for (int i = 0; i < vec.size(); i++)
        {
			animations.push_back(vec[i]->getName().buffer());
		}
	}
	return animations;
}

void SpineAnimator::render(Camera* camera)
{
    this->camera = camera;
    updateVertices();
}

Result<void*> SpineAnimator::setAnimation(int index,bool loop)
{
    if (animation_state != nullptr)
    {
        auto& animations = skeleton->getData()->getAnimations();
        if (index < 0 || index >= animations.size())
        {
			return Result<void*>(false,"Invalid index : "+ std::to_string(index));
		}
		animation_state->setAnimation(0, animations[index], true);
        animation_index = index;
		return Result<void*>();
	}
    return Result<void*>(false,"Current animation state is null !");
}

Result<void*> SpineAnimator::setAnimation(const std::string& name, bool loop)
{
    return Result<void*>(); // TODO
}

void SpineAnimator::awake()
{
	auto animationdata = ResourceMgr::get_instance().loadFromName<SpineAnimationData>(spine_animation_name);
	if (animationdata != nullptr)
	{
		skeleton = new spine::Skeleton(animationdata->get_skeleton_data());
		animation_state =  new spine::AnimationState(animationdata->get_animation_state_data());
	}
	else {
		Debug::logWarning() << "SpineAnimator::awake: SpineAnimationData load failed! : " << spine_animation_name << "\n";
	}
}

void SpineAnimator::afterUpdate()
{
	if (animation_state != nullptr && skeleton != nullptr)
	{
		animation_state->update(Time::deltaTime/1000);
		animation_state->apply(*skeleton);
		skeleton->updateWorldTransform();
	}
}

void SpineAnimator::updateVertices()
{    
    spine::Vector<float> spine_vertices;
    // For each slot in the draw order array of the skeleton
    for (size_t i = 0, n = skeleton->getSlots().size(); i < n; ++i) {
        vertices.clear();
        indices.clear();
        Slot* slot = skeleton->getDrawOrder()[i];

        // Fetch the currently active attachment, continue
        // with the next slot in the draw order if no
        // attachment is active on the slot
        Attachment* attachment = slot->getAttachment();
        if (!attachment) continue;

        // Fetch the blend mode from the slot and
        // translate it to the engine blend mode
        BlendMode engineBlendMode;
        switch (slot->getData().getBlendMode()) {
        case BlendMode_Normal:
            engineBlendMode = BlendMode_Normal;
            break;
        case BlendMode_Additive:
            engineBlendMode = BlendMode_Additive;
            break;
        case BlendMode_Multiply:
            engineBlendMode = BlendMode_Multiply;
            break;
        case BlendMode_Screen:
            engineBlendMode = BlendMode_Screen;
            break;
        default:
            // unknown Spine blend mode, fall back to
            // normal blend mode
            engineBlendMode = BlendMode_Normal;
        }

        // Calculate the tinting color based on the skeleton's color
        // and the slot's color. Each color channel is given in the
        // range [0-1], you may have to multiply by 255 and cast to
        // and int if your engine uses integer ranges for color channels.
        Color skeletonColor = skeleton->getColor();
        Color slotColor = slot->getColor();
        Color tint(skeletonColor.r * slotColor.r, skeletonColor.g * slotColor.g, skeletonColor.b * slotColor.b, skeletonColor.a * slotColor.a);
        Color attachmentColor;
        // Fill the vertices array, indices, and texture depending on the type of attachment
        Texture2D* texture = nullptr;
        if (attachment->getRTTI().isExactly(RegionAttachment::rtti)) {
            // Cast to an spRegionAttachment so we can get the rendererObject
            // and compute the world vertices
            RegionAttachment* regionAttachment = (RegionAttachment*)attachment;

            // Ensure there is enough room for vertices
            spine_vertices.setSize(8, 0.f);

            // Computed the world vertices positions for the 4 vertices that make up
            // the rectangular region attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions
            // will be written directoy into the vertices array, with a stride of sizeof(Vertex)
            regionAttachment->computeWorldVertices(slot->getBone(), spine_vertices.buffer(), 0, 2);
            attachmentColor = regionAttachment->getColor();
            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            texture2D = (Texture2D*)((AtlasRegion*)regionAttachment->getRendererObject())->page->getRendererObject();

            // copy color and UVs to the vertices
            for (size_t j = 0, l = 0; j < 4; j++, l += 2) {
                vertices.push_back({ spine_vertices[l],spine_vertices[l + 1],0.f,regionAttachment->getUVs()[l],regionAttachment->getUVs()[l+1] });
            }

            // set the indices, 2 triangles forming a quad
            indices = { 0, 1, 2, 2, 3, 0 };
        }
        else if (attachment->getRTTI().isExactly(MeshAttachment::rtti)) {
            // Cast to an MeshAttachment so we can get the rendererObject
            // and compute the world vertices
            MeshAttachment* mesh = (MeshAttachment*)attachment;

            // Ensure there is enough room for vertices
            spine_vertices.setSize(mesh->getWorldVerticesLength(), 0.f);

            // Computed the world vertices positions for the vertices that make up
            // the mesh attachment. This assumes the world transform of the
            // bone to which the slot (and hence attachment) is attached has been calculated
            // before rendering via Skeleton::updateWorldTransform(). The vertex positions will
            // be written directly into the vertices array, with a stride of sizeof(Vertex)
            size_t numVertices = mesh->getWorldVerticesLength() / 2;
            mesh->computeWorldVertices(*slot, 0, mesh->getWorldVerticesLength(), spine_vertices.buffer(), 0, 2);
            auto& index = mesh->getTriangles();
            attachmentColor = mesh->getColor();
            // Our engine specific Texture is stored in the AtlasRegion which was
            // assigned to the attachment on load. It represents the texture atlas
            // page that contains the image the region attachment is mapped to.
            texture2D = (Texture2D*)((AtlasRegion*)mesh->getRendererObject())->page->getRendererObject();

            // Copy color and UVs to the vertices
            for (size_t j = 0, l = 0; j < numVertices; j++, l += 2) {
                vertices.push_back({ spine_vertices[l],spine_vertices[l+1],0.f,mesh->getUVs()[l],mesh->getUVs()[l+1]});
            }


            // set the indices, 2 triangles forming a quad
            for (int j=0;j<index.size();++j)
            {
				indices.push_back(index[j]);
			}
        }
        int r = (int)(tint.r * attachmentColor.r * 255);
        int g = (int)(tint.g * attachmentColor.g * 255);
        int b = (int)(tint.b * attachmentColor.b * 255);
        int a = (int)(tint.a * attachmentColor.a * 255);
        color = {r,g,b,a};
        // Draw the mesh we created for the attachment
        RenderWidget::getCurrentWidget().drawMesh(this,this->camera);
    }
}

void SpineAnimator::createIndices()
{
    indices.clear();
    indices = { 0, 1, 2, 2, 3, 0 };
}

void SpineAnimator::reset()
{
    delete skeleton;
    skeleton = nullptr;
    delete animation_state;
    animation_state = nullptr;
    animation_index = 0;
    spine_animation_name = "";
}

void SpineAnimator::serialize(PHString& str)
{
    str.appendLine(std::to_string(componentType));
    IRenderable::serialize(str);
    str.appendLine(spine_animation_name);
}

void SpineAnimator::deserialize(std::stringstream& ss)
{
    IRenderable::deserialize(ss);
    std::string line;
    std::getline(ss, line);
    set_spine_animation_name(line);
}

void SpineAnimator::set_property(Property* property, void* value)
{
	if (property->get_name() == "SpineAnimationName")
	{
		set_spine_animation_name(*(std::string*)value);
	}
    else if (property->get_name() == "Animation")
    {
        setAnimation(*(int*)value,true);
    }
    else
	{
		IRenderable::set_property(property, value);
	}
}
