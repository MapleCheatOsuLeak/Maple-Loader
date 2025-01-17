#include "Game.h"

#include "../Utilities/Security/xorstr.hpp"
#include "../Utilities/Textures/TextureLoader.h"
#include "../UI/Assets/Textures.h"

Game::Game(unsigned int id, const std::string& name)
{
	this->id = id;
	this->name = name;
}

Game::~Game()
{
	if (iconTexture)
		TextureLoader::FreeTexture(iconTexture);

	if (bannerTexture)
		TextureLoader::FreeTexture(bannerTexture);
}

unsigned int Game::GetID()
{
	return id;
}

const std::string& Game::GetName()
{
	return name;
}

void* Game::GetIconTexture()
{
	if (!iconTexture)
	{
		iconTexture = TextureLoader::LoadTextureFromURL(xorstr_("https://maple.software/assets/games/icons/") + std::to_string(id) + xorstr_(".png"));
		if (!iconTexture)
			iconTexture = TextureLoader::LoadTextureFromMemory(Textures::DefaultGameIcon, Textures::DefaultGameIconSize);
	}

	return iconTexture;
}

void* Game::GetBannerTexture()
{
	if (!bannerTexture)
	{
		bannerTexture = TextureLoader::LoadTextureFromURL(xorstr_("https://maple.software/assets/games/banners/") + std::to_string(id) + xorstr_(".png"));
		if (!bannerTexture)
			bannerTexture = TextureLoader::LoadTextureFromMemory(Textures::DefaultBanner, Textures::DefaultBannerSize);
	}

	return bannerTexture;
}
