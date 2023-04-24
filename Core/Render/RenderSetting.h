#pragma once

#include <map>
#include <vector>
#include "Core/Core/ISerializable.h"
#include "Core/Core/GameObject.h"
#include "Core/Core/Renderer.h"
#include "Core/Utils/Vector2D.h"
#include "renderwidget.h"

const std::string RENDER_SETTING_PREFIX = "RenderSetting:";
const std::string RENDER_SETTING_POSTFIX = "RenderSettingEnd";
const std::string RENDER_LAYER_PREFIX = "RenderLayer:";
const std::string RENDER_LAYER_POSTFIX = "RenderLayerEnd";
const std::string RENDER_RESOLUTION_PREFIX = "RenderResolution:";
const std::string RENDER_RESOLUTION_POSTFIX = "RenderResolutionEnd";

class RenderSetting : public ISerializable
{
public:
	RenderSetting();

	// Inherited via ISerializable
	virtual void serialize(PHString& str) override;
	virtual void deserialize(std::stringstream& ss) override;

	//getters and setters
	const std::map<int, std::string>& get_render_layers() const;
	Vector2D getCurrentResolution() const;
	void setCurrentResolution(const Vector2D& value);

	static RenderSetting* getDefaultSetting();
	bool addRenderLayer(int order,const std::string& name);
	bool removeRenderLayer(int order);
	void registerRenderer(Renderer* renderer);
	void unregisterRenderer(Renderer* renderer);

private:
	void refreshLater();
	void refreshAll();
	void addRendererWithChild(GameObject* gameObj);
	void reset2Default(int layer);
	std::map<int, std::vector<GameObject*>> render_order_map;

	//need to serialize
	std::map<int, std::string> render_layers;
	Vector2D current_resolution;


	bool refresh_later = false;

	friend class RenderWidget;
	friend class Renderer;
};

