# YRenderLab
**Personal rendering pipline**

![1578831093943](asset/1578831093943.png)

## PipLine WorkFlow

- [x] diffuse_bsdf material
- [x] standardPbr material
- [x] Raster
  - [x] ForwardRaster
  - [ ] DeferredRaster
- [x] Architecture Build 
- [x] module loading
- [x] Normal Mapping
- [x] Sky Box
- [x] IBL
  - [x] Diffuse
  - [x] specular
- [x] CubeMap
- [x]  Light
  - [x] Direction Light
  - [ ] Point light
- [ ] Shadow
  - [ ]  CSM/PCSS
  - [ ]  距离场阴影
- [ ] Matrix-MainCol

### SoftRHI
- [x] Coordinate Transformation
- [x] Matrix Caculation
- [x] Half Space Rasterization
- [x] BackFace Culling 
- [x] Homogeneous Culling
- [x] Texture Mapping
- [x] Barycentric Interpolation
- [x] Camera controller


### Path Tracer
Cpu implement

- [x] Building frameWork
- [x] Camera Trace
- [x] Antialiasing
- [x] Diffuse Material
- [ ] Metal Material

### Fix

- [ ] VAO-Mesh容器，Texture-Image容器清理
- [x] delete Vertex shader  Error 



### Tips

*Gbuffer layout*

**GLTexture::TexTureformat::TEX_GL_RGBA32F, //pixpos + roughness
GLTexture::TexTureformat::TEX_GL_RGBA32F, //normal + metallic
GLTexture::TexTureformat::TEX_GL_RGBA32F  //albedo + ao**



