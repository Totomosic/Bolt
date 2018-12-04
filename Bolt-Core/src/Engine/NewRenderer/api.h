#pragma once
// Render per layer

RenderGroup group;
group.Material;
group.Renderables;

RenderPass pass;
pass.RenderTarget;
pass.ViewMatrix;
pass.ProjectionMatrix;
pass.RenderGroups;

RenderContext context;
context.Lights;
context.StaticMeshes;

GlobalRenderer::Render(pass, context);