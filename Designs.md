### 程序流程

- 统一入口为调用Application的Init函数
  - 函数内部创建对应的RenderRHI和Application的窗口
  - 上述创建成功后创建RenderPipeline，该Pipeline负责实际渲染逻辑
- 逻辑更新统一接口为Update与Render
  - Update用于更新以下逻辑
    - 窗口输入处理逻辑
    - World数据更新逻辑
  - Render用于提交实际的渲染命令





