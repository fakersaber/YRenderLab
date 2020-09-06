# YRenderLab

**Personal rendering pipline**

![1578831093943](asset/1578831093943.png)

## TODO LIST

- [ ] Vulkan Pipeline Waraper
- [ ] Render Graph
- [ ] imgui
- [ ] PTR
  - Irradiance volume
  - dynamic diffuse GI
- [ ] LPV



### Tips

*Gbuffer layout*

**GLTexture::TexTureformat::TEX_GL_RGBA32F, //pixpos + roughness**

**GLTexture::TexTureformat::TEX_GL_RGBA32F, //normal + metallic**

**GLTexture::TexTureformat::TEX_GL_RGBA32F  //albedo + ao**



