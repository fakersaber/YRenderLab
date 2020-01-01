# YRenderLab
**Personal rendering pipline**

## PipLine WorkFlow

- [x] diffuse_bsdf material
- [x] physics Blinn Phong
- [x] standardPbr material
- [x] ForwardRaster pipline
- [x] Architecture Build 
- [x] module loading
- [x] Normal Mapping
- [x] Sky Box
- [x] Direct Light
- [x] IBL
  - [x] Diffuse
  - [x] specular
- [x] CubeMap
- [ ] Point Light
- [ ] Shadow
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
- [x] delete Vertex shader  Error ?
- [ ] shader指令不同造成的崩溃？



### Note

- 3dmax修改模型时贴图对应关系（使用默认材质导出，所以在assimp加载模型时贴图名称要改改..）
  -  Diffuse --> basecolor
  -  specular --> metallic
  -  emitter --> roughness
  -  height --> normal
  -  ??? --> ao （还没决定）

