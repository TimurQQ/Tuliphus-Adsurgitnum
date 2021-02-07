#include "pch.h"
#include "DynamicLayer.h"

DynamicLayer::DynamicLayer(sf::Uint16 width, sf::Uint16 height, sf::Uint16 tile_width, sf::Uint16 tile_height, sf::Uint8 patch_size,
	std::vector<std::unique_ptr<sf::Texture>>* tileset_textures, std::vector<tmx::MapLoader::TileInfo>* tile_info,
	sf::RenderTarget* render_window)
	: width_(width),
	height_(height),
	tile_width(tile_width),
	tile_height(tile_height),
	patch_size_(patch_size),
	tileset_textures_(tileset_textures),
	tile_info(tile_info),
	render_window_(render_window)
{}

DynamicLayer::~DynamicLayer() {
	objects_.clear();
	layerSets.clear();
}

DynamicLayer& DynamicLayer::operator=(const DynamicLayer& a) {
	if (this != &a) {
		tileset_textures_ = a.tileset_textures_;
		tile_info = a.tile_info;
		width_ = a.width_;
		height_ = a.height_;
		tile_width = a.tile_width;
		tile_height = a.tile_height;
		render_window_ = a.render_window_;
	}
	return *this;
}

void DynamicLayer::AddObject(sf::Vector2f pos, sf::Vector2f size, float rot_angle, int gid) {
	tmx::MapObject object;
	object.setPosition(sf::Vector2f(0.f, 32.f));
	object.setRotation(rot_angle);
	if (gid != -1) {
		std::cout << "Found object with tile GID " + std::to_string(gid) << std::endl;

		object.move(0.f, static_cast<float>(-tile_height)); //offset for tile origins being at the bottom in Tiled
		const sf::Uint16 x = static_cast<sf::Uint16>(object.getPosition().x / tile_width);
		std::cout << object.getPosition().y;
		const sf::Uint16 y = static_cast<sf::Uint16>(object.getPosition().y / tile_height);

		sf::Vector2f offset(object.getPosition().x - (x * tile_width), (object.getPosition().y - (y * tile_height)));

		object.setQuad(AddTileToLayer(x, y, gid, offset));
		object.setShapeType(tmx::MapObjectShape::Tile);

		tmx::MapLoader::TileInfo info = (*tile_info)[gid];
		//create bounding poly
		float width = static_cast<float>(info.Size.x);
		float height = static_cast<float>(info.Size.y);

		object.addPoint(sf::Vector2f() + sf::Vector2f(0, 16.f));
		object.addPoint(sf::Vector2f(width, 0.f) + sf::Vector2f(0, 16.f));
		object.addPoint(sf::Vector2f(width, height) + sf::Vector2f(0, 16.f));
		object.addPoint(sf::Vector2f(0.f, height) + sf::Vector2f(0, 16.f));
		object.setSize(sf::Vector2f(width, height));
	}
	else {
		object.addPoint(sf::Vector2f());
		object.addPoint(sf::Vector2f(size.x, 0.f));
		object.addPoint(sf::Vector2f(size.x, size.y));
		object.addPoint(sf::Vector2f(0.f, size.y));
		object.setSize(size);
	}
	object.setParent("action_layer");
	sf::Color debugColour;
	debugColour = sf::Color(127u, 127u, 127u);
	debugColour.a = static_cast<sf::Uint8>(255.f * opacity_);
	object.createDebugShape(debugColour);
	object.createSegments();
	render_window_->draw(*this);
	object.setPosition(pos);
	objects_.push_front(object);
}
