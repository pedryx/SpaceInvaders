#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <map>
#include <string>
#include <SFML/Graphics.hpp>

/**
 * Represent a manager of textures.
 * (Singleton)
 */
class TextureManager
{
public:
	/**
	 * Get instance of texture manager.
	 * 
	 */
	static TextureManager& getInstance()
	{
		static TextureManager instance;

		return instance;
	}

	/**
	 * Load new texture to manager.
	 * 
	 * \param path Path to a texture.
	 * \param name Name of a texture.
	 */
	void load(const std::string& path, const std::string& name)
	{
		auto it = textures_.insert({ name, sf::Texture() }).first;
		(*it).second.loadFromFile(path);
	}

	/**
	 * Get loaded texture from manager.
	 * 
	 * \param name Name of texture to get.
	 * \return Texture with specific name.
	 */
	sf::Texture& get(const std::string& name)
	{
		return textures_[name];
	}
private:
	TextureManager() {}

	/**
	 * Contains all loaded textures.
	 */
	std::map<std::string, sf::Texture> textures_;
};

#endif // !TEXTURE_MANAGER_HPP