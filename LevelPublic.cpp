
#include "pch.h"
#include "Level.h"
#include "DynamicLayer.h"

Level::Level(const std::vector<std::string>& sub_dirs,  const std::string& mapFilename, sf::RenderWindow* render_window)
	:render_window_(render_window)
{
	Level::LoadStaticMap(sub_dirs, mapFilename);
	Level::LoadPinnedText();
	Level::CreateDynamicLayer();
	
	int max_base_id = Level::GetBasesCount();
	colonies_ = std::vector<Colony>(max_base_id + 1);
	Level::SetMapValues();
	Level::LoadColonies();
	Level::LoadResourcePoints();
	ReloadShapes();
}

Level::~Level() {
	colonies_.clear();
}

void Level::ShowDebug() {
	static_map_.drawLayer(*render_window_, tmx::MapLayer::DrawType::Debug);
	for (const auto& object : dynamic_objs_.objects())
		if (static_map_.bounds().intersects(object.getAABB()))
			object.drawDebugShape(*render_window_);
}

void Level::Draw() {
	render_window_->draw(static_map_);
	render_window_->draw(dynamic_objs_);
	for (auto& colony : colonies_) {
		colony.DrawButtons(render_window_);
	}
	render_window_->draw(pinned_text_);
}

void Level::UpdateState(bool& show_debug) {
	Level::ProcessEvents(show_debug);
	Level::UpdateColoniesState();
	UpdateView();
	frame_clock_.restart();
}
