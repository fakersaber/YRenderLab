#pragma once

class RenderScene;

class IPipeline {

public:
	IPipeline() {}
	virtual ~IPipeline() = default;
	virtual void BeginFrame(RenderScene* World) = 0;
	virtual void Render() = 0;
	virtual void EndFrame() = 0;
};