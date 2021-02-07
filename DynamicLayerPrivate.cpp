#include "pch.h"
#include "DynamicLayer.h"

void DynamicLayer::setShader(const sf::Shader& shader)
{
	m_shader = &shader;
}

void DynamicLayer::cull(const sf::FloatRect& bounds)
{
	for (auto& ls : layerSets)
		ls.second->cull(bounds);
}

void DynamicLayer::draw(sf::RenderTarget& rt, sf::RenderStates states) const
{
	if (!visible) return; //skip invisible layers
	states.shader = m_shader;
	for (const auto& ls : layerSets) {
		rt.draw(*ls.second, states);
	}
}

void DynamicLayer::flipY(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
	sf::Vector2f tmp = *v0;
	v0->y = v2->y;
	v1->y = v2->y;
	v2->y = tmp.y;
	v3->y = v2->y;
}
void DynamicLayer::flipX(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
	sf::Vector2f tmp = *v0;
	v0->x = v1->x;
	v1->x = tmp.x;
	v2->x = v3->x;
	v3->x = v0->x;
}
void DynamicLayer::flipD(sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
	sf::Vector2f tmp = *v1;
	v1->x = v3->x;
	v1->y = v3->y;
	v3->x = tmp.x;
	v3->y = tmp.y;
}

void DynamicLayer::doFlips(std::bitset<3> bits, sf::Vector2f* v0, sf::Vector2f* v1, sf::Vector2f* v2, sf::Vector2f* v3) {
	if (!bits.test(0) && !bits.test(1) && !bits.test(2)) {
		//Shortcircuit tests for nothing to do
		return;
	}
	else if (bits.test(0) && !bits.test(1) && !bits.test(2)) {
		//001
		flipY(v0, v1, v2, v3);
	}
	else if (!bits.test(0) && bits.test(1) && !bits.test(2)) {
		//010
		flipX(v0, v1, v2, v3);
	}
	else if (bits.test(0) && bits.test(1) && !bits.test(2)) {
		//011
		flipY(v0, v1, v2, v3);
		flipX(v0, v1, v2, v3);
	}
	else if (!bits.test(0) && !bits.test(1) && bits.test(2)) {
		//100
		flipD(v0, v1, v2, v3);
	}
	else if (bits.test(0) && !bits.test(1) && bits.test(2)) {
		//101
		flipX(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
	else if (!bits.test(0) && bits.test(1) && bits.test(2)) {
		//110
		flipY(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
	else if (bits.test(0) && bits.test(1) && bits.test(2)) {
		//111
		flipY(v0, v1, v2, v3);
		flipX(v0, v1, v2, v3);
		flipD(v0, v1, v2, v3);
	}
}

std::vector<unsigned char> DynamicLayer::intToBytes(sf::Uint32 paramInt)
{
	std::vector<unsigned char> arrayOfByte(4);
	for (int i = 0; i < 4; i++)
		arrayOfByte[i] = (paramInt >> (i * 8));
	return arrayOfByte;
}

std::pair<sf::Uint32, std::bitset<3> > DynamicLayer::resolveRotation(sf::Uint32 gid)
{
	const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
	const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
	const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
	std::vector<unsigned char> bytes = intToBytes(gid);
	sf::Uint32 tileGID = bytes[0] | bytes[1] << 8 | bytes[2] << 16 | bytes[3] << 24;
	bool flipped_diagonally = (tileGID & FLIPPED_DIAGONALLY_FLAG);
	bool flipped_horizontally = (tileGID & FLIPPED_HORIZONTALLY_FLAG);
	bool flipped_vertically = (tileGID & FLIPPED_VERTICALLY_FLAG);
	std::bitset<3> b;
	b.set(0, flipped_vertically);
	b.set(1, flipped_horizontally);
	b.set(2, flipped_diagonally);
	tileGID &= ~(FLIPPED_HORIZONTALLY_FLAG |
		FLIPPED_VERTICALLY_FLAG |
		FLIPPED_DIAGONALLY_FLAG);
	return std::pair<sf::Uint32, std::bitset<3> >(tileGID, b);
}

tmx::TileQuad* DynamicLayer::AddTileToLayer(sf::Uint16 x, sf::Uint16 y, sf::Uint32 gid, const sf::Vector2f& offset) {
	sf::Uint8 opacity = static_cast<sf::Uint8>(255.f * opacity_);
	sf::Color colour = sf::Color(255u, 255u, 255u, opacity);
	//Get bits and tile id
	std::pair<sf::Uint32, std::bitset<3> > idAndFlags = resolveRotation(gid);
	gid = idAndFlags.first;
	//update the layer's tile set(s)
	sf::Vertex v0, v1, v2, v3;
	//applying half pixel trick avoids artifacting when scrolling
	v0.texCoords = (*tile_info)[gid].Coords[0] + sf::Vector2f(0.5f, 0.5f);
	v1.texCoords = (*tile_info)[gid].Coords[1] + sf::Vector2f(-0.5f, 0.5f);
	v2.texCoords = (*tile_info)[gid].Coords[2] + sf::Vector2f(-0.5f, -0.5f);
	v3.texCoords = (*tile_info)[gid].Coords[3] + sf::Vector2f(0.5f, -0.5f);
	//flip texture coordinates according to bits set
	doFlips(idAndFlags.second, &v0.texCoords, &v1.texCoords, &v2.texCoords, &v3.texCoords);
	v0.position = sf::Vector2f(static_cast<float>(tile_width * x), static_cast<float>(tile_height * y));
	v1.position = sf::Vector2f(static_cast<float>(tile_width * x) + (*tile_info)[gid].Size.x, static_cast<float>(tile_height * y));
	v2.position = sf::Vector2f(static_cast<float>(tile_width * x) + (*tile_info)[gid].Size.x, static_cast<float>(tile_height * y) + (*tile_info)[gid].Size.y);
	v3.position = sf::Vector2f(static_cast<float>(tile_width * x), static_cast<float>(tile_height * y) + (*tile_info)[gid].Size.y);
	//offset tiles with size not equal to map grid size
	sf::Uint16 tileHeight = static_cast<sf::Uint16>((*tile_info)[gid].Size.y);
	if (tileHeight != tile_height)
	{
		float diff = static_cast<float>(tile_height - tileHeight);
		v0.position.y += diff;
		v1.position.y += diff;
		v2.position.y += diff;
		v3.position.y += diff;
	}
	v0.color = colour;
	v1.color = colour;
	v2.color = colour;
	v3.color = colour;
	v0.position += offset;
	v1.position += offset;
	v2.position += offset;
	v3.position += offset;
	sf::Uint16 id = (*tile_info)[gid].TileSetId;
	if (layerSets.find(id) == layerSets.end())
	{
		//create a new layerset for texture
		layerSets.insert(std::make_pair(id, std::make_shared<tmx::LayerSet>(*(*tileset_textures_)[id], patch_size_, sf::Vector2u(width_, height_), sf::Vector2u(tile_width, tile_height))));
	}
	//add tile to set
	std::cout << layerSets[id];
	return layerSets[id]->addTile(v0, v1, v2, v3, x, y);
}
