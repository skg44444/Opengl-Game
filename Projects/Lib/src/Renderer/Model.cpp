#include "libpch.h"
#include "Platform/tinyobjmodel/tinyobjModel.h"

namespace Lib
{
	std::shared_ptr<Model> Model::CreateModel(const std::string& filepath)
	{
		return std::make_shared<TinyObjModel>(filepath);
	}
	std::shared_ptr<Model> Model::CreateTerrain()
	{
		return std::make_shared<TerrainModel>();
	}
}