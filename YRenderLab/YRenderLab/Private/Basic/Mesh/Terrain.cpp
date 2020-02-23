#include <Public/Basic/Mesh/Terrain.h>

Terrain::Terrain(){

	constexpr std::size_t Terrian_Width = 20;
	constexpr std::size_t Terrian_Hiehgt = 20;

	position.reserve(Terrian_Width * Terrian_Hiehgt * 4);
	indice.reserve(Terrian_Width * Terrian_Hiehgt * 6);

	//for (std::size_t x = 0; x < Terrian_Width; ++x) {
	//	const float x_offset = -static_cast<float>(Terrian_Width) + 2 * x;  //x_offset [-20.f,20.f]
	//	for (std::size_t y = 0; y < Terrian_Hiehgt; ++y) {
	//		const float y_offset = -static_cast<float>(Terrian_Hiehgt) + 2 * y;
	//		verts.push_back(
	//			{ 
	//				{1.0f + x_offset, 0.0f, 1.0f + y_offset},  //x [-19,21]  z [-19,21]
	//				{1.0f, 0.0f} 
	//			}
	//		);

	//		verts.push_back(
	//			{ 
	//				{1.0f + x_offset, 0.0f, -1.0f + y_offset}, 
	//				{1.0f, 1.0f} 
	//			}
	//		);

	//		verts.push_back(
	//			{ 
	//				{-1.0f + x_offset, 0.0f, -1.0f + y_offset},  //texcoords”¶∏√ «0,0
	//				{0.0f, 1.0f} 
	//			}
	//		);


	//		verts.push_back(
	//			{ 
	//				{-1.0f + x_offset, 0.0f, 1.0f + y_offset}, // z [-19,21]
	//				{0.0f, 0.0f} 
	//			}
	//		);

	//		const auto index_base =
	//			static_cast<std::uint32_t>(4 * (y + x * terrian_y_max));
	//		indices.push_back(index_base);
	//		indices.push_back(index_base + 1);
	//		indices.push_back(index_base + 3);
	//		indices.push_back(index_base + 1);
	//		indices.push_back(index_base + 2);
	//		indices.push_back(index_base + 3);
	//	}
}

void Terrain::RenderPrimitive(const std::shared_ptr<Raster>& raster, const YGM::Transform & model){

}

void Terrain::RenderShadowPrimitive(const std::shared_ptr<ShadowGen>& shadowgen, const YGM::Transform & model){

}
