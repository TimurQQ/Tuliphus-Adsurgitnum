#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <tmx/MapLoader.hpp>
#include <SFML/Graphics/Export.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Window/Window.hpp>

class DynamicLayer : public sf::Drawable {
	class TileInfo;
public:
	DynamicLayer() {}

	DynamicLayer(sf::Uint16 width, sf::Uint16 height, sf::Uint16 tile_width, sf::Uint16 tile_height,
		sf::Uint8 patch_size, std::vector<std::unique_ptr<sf::Texture>>* tileset_textures,
		std::vector<tmx::MapLoader::TileInfo>* tile_info, sf::RenderTarget* render_window);
	
	~DynamicLayer();

	std::list<tmx::MapObject>& objects() { return objects_; }

	DynamicLayer& operator=(const DynamicLayer& a);

	void AddObject(sf::Vector2f pos, sf::Vector2f size, float rot_angle, int gid = -1);
protected:
	// pass
private:
	const sf::Shader* m_shader = nullptr;
	float opacity_ = 1.f;
	sf::RenderTarget* render_window_ = nullptr;
	bool visible = true;
	sf::Uint16 tile_height = 1u, tile_width = 1u;
	sf::Uint16 width_ = 1u, height_ = 1u;
	std::vector<std::unique_ptr<sf::Texture>>* tileset_textures_ = nullptr;
	const sf::Uint8 patch_size_ = 10u;
	std::vector<tmx::MapLoader::TileInfo>* tile_info = nullptr;
	tmx::MapTiles tiles;
	std::list<tmx::MapObject> objects_;
	std::map<sf::Uint16, std::shared_ptr<tmx::LayerSet>> layerSets;
	
	// Sets the shader which will be used when drawing this layer
	void setShader(const sf::Shader& shader);
	// Used to cull patches outside the visible area
	void cull(const sf::FloatRect& bounds);
	void draw(sf::RenderTarget& rt, sf::RenderStates states) const override;
	tmx::TileQuad* AddTileToLayer(sf::Uint16 x, sf::Uint16 y, sf::Uint32 gid, const sf::Vector2f& offset);
	void flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
	void flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
	void flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);

	void doFlips(std::bitset<3> bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3);
	std::pair<sf::Uint32, std::bitset<3> > resolveRotation(sf::Uint32 gid);
	std::vector<unsigned char> intToBytes(sf::Uint32 paramInt);
};