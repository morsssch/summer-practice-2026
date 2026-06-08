#pragma once

#include <SFML/Graphics.hpp>

struct RenderResources {
  sf::Texture tilesetTex;
  sf::Texture heroTex;
  sf::Texture bgTex;
  sf::Texture grassTex;
  sf::Font font;
  bool fontLoaded = false;
  bool grassLoaded = false;
  sf::Texture mushroomTex;
  bool mushroomLoaded = false;
  sf::Texture slimeTex;
  bool slimeLoaded = false;
  sf::Texture torchTex;
  bool torchLoaded = false;
  sf::Texture spikeTex;
  bool spikeLoaded = false;
  sf::Texture plateTex;
  bool plateLoaded = false;
  sf::Texture platePressedTex;
  bool platePressedLoaded = false;
  sf::Texture doorClosedTex;
  bool doorClosedLoaded = false;
  sf::Texture doorOpeningTex;
  bool doorOpeningLoaded = false;
  sf::Texture stoneTex;
  bool stoneLoaded = false;
  sf::Texture orbDropTex;
  bool orbDropLoaded = false;
  sf::Texture orbCollectedTex;
  bool orbCollectedLoaded = false;
  sf::Texture potionTex;
  bool potionLoaded = false;
  sf::RenderTexture lightMap;
  sf::Texture hudHeartFullTex;
  sf::Texture hudHeartEmptyTex;
  sf::Texture hudHeroIconTex;
  sf::Texture hudLostHeartTex;
  bool hudHeartsLoaded = false;
  bool hudHeroIconLoaded = false;
  bool hudLostHeartLoaded = false;
};
