#pragma once
#include <array>
#include <curl/curl.h>
#include <filesystem>
#include <future>
#include <memory>
#include <string>
#include <vector>

namespace ImOsm {
class TileGrabberUrl {
public:
  TileGrabberUrl();

  void grab(int minZ, int maxZ, float minLat, float maxLat, float minLon,
            float maxLon);

private:
  std::string _clientName{"curl"};
  std::string _tileProvider{"http://a.tile.openstreetmap.org/"};
  std::string _tileExtension{".png"};
  std::string _tileSavePath{std::filesystem::current_path().string() +
                            "/tiles"};
  int _maxParallelGrab{4};

  struct Tile {
    struct Remote {
      std::vector<std::byte> blob;
      CURLcode code{CURLE_OK};
    };
    std::array<int, 3> zxy{0, 0, 0};
    std::future<Remote> future;
  };

  struct GrabData {
    std::vector<Tile> tiles;
  };
  std::future<GrabData> _grabFuture;

  GrabData onLaunchGrab(int zmin, int zmax, int xmin, int xmax, int ymin,
                        int ymax);
  Tile::Remote onHandleRequest(const std::array<int, 3> &zxy);
  static size_t onPullResponse(void *data, size_t size, size_t nmemb,
                               void *userp);

  void saveFile(Tile &tile);
};
}; // namespace ImOsm
