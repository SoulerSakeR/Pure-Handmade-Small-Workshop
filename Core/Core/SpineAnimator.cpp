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
#include <spine/Skin.h>

using namespace spine;

unsigned short quadIndices[] = { 0, 1, 2, 2, 3, 0 };

SpineAnimator::SpineAnimator(GameObject* gameobj): IRenderable(gameobj)
{
	componentType = ComponentType::SPINE_ANIMATOR;
    spine_animation_name = "None";
    properties["Color"]->is_visible = false;
    auto spine_animation_name_property = new Property("Spine Animation Name", &spine_animation_name, Property::ANIMATION, this);
    spine_animation_name_property->set_property_func<string>(&SpineAnimator::get_spine_animation_name, &SpineAnimator::set_spine_animation_name, this);
    properties.emplace(spine_animation_name_property);
    animation_index = 0;
    auto animation_property = new Property("Animation", &animation_index, Property::ANIMATION_COMBOBOX, this);
    animation_property->set_property_func<int>(&SpineAnimator::get_animation_index, &SpineAnimator::set_animation_index, this);
    properties.emplace(animation_property);
    skin_index = 0;
    auto skin_property = new Property("Skin", &skin_index, Property::SKIN_COMBOBOX, this);
    skin_property->set_property_func<int>(&SpineAnimator::get_skin_index, &SpineAnimator::set_skin_index, this);
    properties.emplace(skin_property);
    loop = true;
    auto loop_property = new Property("Loop", &loop, Property::BOOL, this);
    loop_property->set_property_func<bool>(&SpineAnimator::get_loop, &SpineAnimator::set_loop, this);
    properties.emplace(loop_property);
}

std::string SpineAnimator::get_spine_animation_name() const
{
    return spine_animation_name;
}

bool SpineAnimator::set_spine_animation_name(const std::string& name)
{
    if (name == "None")
    {
		reset();
        onPropertyChanged(properties["Spine Animation Name"]);
        onPropertyChanged(properties["Animation"]);
		return true;
	}
	auto animationdata = ResourceMgr::get_instance().loadFromName<SpineAnimationData>(name);
	if (animationdata != nullptr)
	{
        reset();
		spine_animation_name = name;
		skeleton = new spine::Skeleton(animationdata->get_skeleton_data());
		animation_state = new spine::AnimationState(animationdata->get_animation_state_data());
        skeleton->updateWorldTransform();
        onPropertyChanged(properties["Spine Animation Name"]);
        onPropertyChanged(properties["Animation"]);
        onPropertyChanged(properties["Skin"]);
		return true;
	}
	return false;
}

int SpineAnimator::get_animation_index() const
{
    return animation_index;
}

void SpineAnimator::set_animation_index(int index)
{
    setAnimation(index, loop);
    onPropertyChanged(properties["Animation"]);
}

int SpineAnimator::get_skin_index() const
{
    return skin_index;
}

void SpineAnimator::set_skin_index(int index)
{
    if (skeleton != nullptr)
    {
		auto& skins = skeleton->getData()->getSkins();
        if (index < 0 || index >= skins.size())
        {
			return;
		}
		skeleton->setSkin(skins[index]);
		skeleton->setSlotsToSetupPose();
		skeleton->updateWorldTransform();
		skin_index = index;
        onPropertyChanged(properties["Skin"]);
	}	
}

bool SpineAnimator::get_loop() const
{
    return loop;
}

void SpineAnimator::set_loop(bool loop)
{
    this->loop = loop;
    onPropertyChanged(properties["Loop"]);
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

std::vector<std::string> SpineAnimator::getAllSkins()
{
    std::vector<std::string> skins;
    if (skeleton != nullptr)
    {
		spine::Vector<spine::Skin*>& vec = skeleton->getData()->getSkins();
        for (int i = 0; i < vec.size(); i++)
        {
            skins.push_back(vec[i]->getName().buffer());
        }
    }
    return skins;
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
		animation_state->setAnimation(0, animations[index], loop);
        animation_index = index;
		return Result<void*>();
	}
    return Result<void*>(false,"Current animation state is null !");
}

Result<void*> SpineAnimator::setAnimation(const std::string& name, bool loop)
{
    if (animation_state != nullptr)
    {
		auto& animations = skeleton->getData()->getAnimations();
        auto animation = skeleton->getData()->findAnimation(name.c_str());
        if (animation != nullptr)
        {
            animation_state->setAnimation(0, animation, loop);
        }
		return Result<void*>(false, "Invalid animation name : " + name);
	}
    return Result<void*>(false, "Current animation state is null !");
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
    if (animation_state == nullptr || skeleton == nullptr)
        return;
    vertices.clear();
    indices.clear();
    spine::Vector<float> spine_vertices;
    
    // For each slot in the draw order array of the skeleton
    for (size_t i = 0, n = skeleton->getSlots().size(); i < n; ++i) {
        int size = vertices.size();
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
            assert(size + 3 < vertices.size());
            indices.push_back(size + 0);
            indices.push_back(size+1);
            indices.push_back(size+2);
            indices.push_back(size+2);
            indices.push_back(size+3);
            indices.push_back(size+0);
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
                assert(size + index[j] < vertices.size());
				indices.push_back(size+index[j]);
			}
        }
        int r = (int)(tint.r * attachmentColor.r * 255);
        int g = (int)(tint.g * attachmentColor.g * 255);
        int b = (int)(tint.b * attachmentColor.b * 255);
        int a = (int)(tint.a * attachmentColor.a * 255);
        color = {r,g,b,a};
        // Draw the mesh we created for the attachment
    }
    RenderWidget::getCurrentWidget().drawMesh(this, this->camera);
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
    spine_animation_name = "None";
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

