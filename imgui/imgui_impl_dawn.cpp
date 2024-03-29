// Copyright 2019 Mathieu-Andre Chiasson
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "imgui_impl_dawn.h"
#include "utils/SampleUtils.h"
#include <utils/ComboRenderPipelineDescriptor.h>
#include <utils/DawnHelpers.h>
#include <utils/SystemUtils.h>

#include <cstddef> // for offsetof
#include <vector>

constexpr size_t kMaxVertexCount = 65536;

static dawn::BindGroup bindGroup;
static dawn::Buffer uniformBuffer;
static dawn::RenderPipeline pipeline;
static dawn::Sampler fontSampler;
static dawn::Texture fontTexture;
static std::vector<dawn::Buffer> indexBuffers;
static std::vector<dawn::Buffer> vertexBuffers;

struct alignas(kMinDynamicBufferOffsetAlignment) ShaderData {
    float ProjMtx[4][4];
};

// Functions
bool ImGui_ImplDawn_Init(dawn::Device& device, dawn::Queue& queue)
{
    // Setup back-end capabilities flags
    ImGuiIO& io = ImGui::GetIO();
    io.BackendRendererName = "imgui_impl_dawn";

    // Init Buffers
    {
        dawn::BufferDescriptor descriptor;
        descriptor.size = sizeof(ShaderData);
        descriptor.usage = dawn::BufferUsage::CopyDst | dawn::BufferUsage::Uniform;
        uniformBuffer = device.CreateBuffer(&descriptor);
    }

    // Init Texture
    {
        unsigned char* pixels;
        int width, height;
        io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

        dawn::TextureDescriptor descriptor;
        descriptor.dimension = dawn::TextureDimension::e2D;
        descriptor.size.width = width;
        descriptor.size.height = height;
        descriptor.size.depth = 1;
        descriptor.arrayLayerCount = 1;
        descriptor.sampleCount = 1;
        descriptor.format = dawn::TextureFormat::RGBA8Unorm;
        descriptor.mipLevelCount = 1;
        descriptor.usage = dawn::TextureUsage::CopyDst | dawn::TextureUsage::Sampled;
        fontTexture = device.CreateTexture(&descriptor);

        dawn::SamplerDescriptor samplerDesc = utils::GetDefaultSamplerDescriptor();
        fontSampler = device.CreateSampler(&samplerDesc);

        dawn::Buffer stagingBuffer = utils::CreateBufferFromData(
            device, pixels, 4 * width * height, dawn::BufferUsage::CopySrc);
        dawn::BufferCopyView bufferCopyView = utils::CreateBufferCopyView(stagingBuffer, 0, 0, 0);
        dawn::TextureCopyView textureCopyView = utils::CreateTextureCopyView(fontTexture, 0, 0, { 0, 0, 0 });
        dawn::Extent3D copySize = { static_cast<uint32_t>(width), static_cast<uint32_t>(height), 1 };

        dawn::CommandEncoder encoder = device.CreateCommandEncoder();
        encoder.CopyBufferToTexture(&bufferCopyView, &textureCopyView, &copySize);

        dawn::CommandBuffer copy = encoder.Finish();
        queue.Submit(1, &copy);
    }

    // Init Pipeline
    {
        dawn::ShaderModule vsModule = utils::CreateShaderModule(device, utils::SingleShaderStage::Vertex, R"(
                #version 450
                layout(set = 0, binding = 0) uniform Constants { mat4 ProjMtx; } c;
                layout(location = 0) in vec2 Position;
                layout(location = 1) in vec2 UV;
                layout(location = 2) in vec4 Color;
                layout(location = 3) out vec2 Frag_UV;
                layout(location = 4) out vec4 Frag_Color;
                void main()
                {
                    Frag_UV = UV;
                    Frag_Color = Color;
                    gl_Position = c.ProjMtx * vec4(Position.xy,0,1);
                })");

        dawn::ShaderModule fsModule = utils::CreateShaderModule(device, utils::SingleShaderStage::Fragment, R"(
                #version 450
                layout(location = 3) in vec2 Frag_UV;
                layout(location = 4) in vec4 Frag_Color;
                layout(set = 0, binding = 1) uniform sampler mySampler;
                layout(set = 0, binding = 2) uniform texture2D myTexture;
                layout(location = 0) out vec4 Out_Color;
                void main()
                {
                    Out_Color = Frag_Color * texture(sampler2D(myTexture, mySampler), Frag_UV.st);
                })");

        dawn::BindGroupLayout bgl = utils::MakeBindGroupLayout(
            device, {
                        { 0, dawn::ShaderStage::Vertex, dawn::BindingType::UniformBuffer },
                        { 1, dawn::ShaderStage::Fragment, dawn::BindingType::Sampler },
                        { 2, dawn::ShaderStage::Fragment, dawn::BindingType::SampledTexture },
                    });

        dawn::PipelineLayout pl = utils::MakeBasicPipelineLayout(device, &bgl);

        utils::ComboRenderPipelineDescriptor descriptor(device);
        descriptor.layout = pl;
        descriptor.vertexStage.module = vsModule;
        descriptor.cFragmentStage.module = fsModule;
        descriptor.cVertexInput.bufferCount = 1;
        descriptor.cVertexInput.cBuffers[0].stride = sizeof(ImDrawVert);
        descriptor.cVertexInput.cBuffers[0].attributeCount = 3;
        descriptor.cVertexInput.cAttributes[0].shaderLocation = 0;
        descriptor.cVertexInput.cAttributes[0].offset = offsetof(ImDrawVert, pos);
        descriptor.cVertexInput.cAttributes[0].format = dawn::VertexFormat::Float2;
        descriptor.cVertexInput.cAttributes[1].shaderLocation = 1;
        descriptor.cVertexInput.cAttributes[1].offset = offsetof(ImDrawVert, uv);
        descriptor.cVertexInput.cAttributes[1].format = dawn::VertexFormat::Float2;
        descriptor.cVertexInput.cAttributes[2].shaderLocation = 2;
        descriptor.cVertexInput.cAttributes[2].offset = offsetof(ImDrawVert, col);
        descriptor.cVertexInput.cAttributes[2].format = dawn::VertexFormat::UChar4Norm;
        descriptor.depthStencilState = &descriptor.cDepthStencilState;
        descriptor.cDepthStencilState.format = dawn::TextureFormat::Depth24PlusStencil8;
        descriptor.cColorStates[0].format = GetPreferredSwapChainTextureFormat();
        descriptor.cColorStates[0].alphaBlend.srcFactor = dawn::BlendFactor::SrcAlpha;
        descriptor.cColorStates[0].alphaBlend.dstFactor = dawn::BlendFactor::OneMinusSrcAlpha;
        descriptor.cColorStates[0].colorBlend.srcFactor = dawn::BlendFactor::SrcAlpha;
        descriptor.cColorStates[0].colorBlend.dstFactor = dawn::BlendFactor::OneMinusSrcAlpha;

        pipeline = device.CreateRenderPipeline(&descriptor);

        dawn::TextureView fontTextureview = fontTexture.CreateView();

        bindGroup = utils::MakeBindGroup(device, bgl, { { 0, uniformBuffer, 0, sizeof(ShaderData) }, { 1, fontSampler }, { 2, fontTextureview } });
    }

    return true;
}

void ImGui_ImplDawn_Shutdown()
{
    bindGroup.Release();
    uniformBuffer.Release();
    pipeline.Release();
    fontSampler.Release();
    fontTexture.Release();
    for (dawn::Buffer& indexBuffer : indexBuffers) {
        indexBuffer.Release();
    }
    for (dawn::Buffer& vertexBuffer : vertexBuffers) {
        vertexBuffer.Release();
    }
}

void ImGui_ImplDawn_NewFrame()
{
}

void ImGui_ImplDawn_RenderDrawData(ImDrawData* draw_data, dawn::Device& device, dawn::RenderPassEncoder& pass)
{
    // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
    ImGuiIO& io = ImGui::GetIO();
    int fb_width = (int)(draw_data->DisplaySize.x * io.DisplayFramebufferScale.x);
    int fb_height = (int)(draw_data->DisplaySize.y * io.DisplayFramebufferScale.y);
    if (fb_width <= 0 || fb_height <= 0)
        return;
    draw_data->ScaleClipRects(io.DisplayFramebufferScale);

    // Setup viewport, orthographic projection matrix
    // Our visible imgui space lies from draw_data->DisplayPps (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
    float L = draw_data->DisplayPos.x;
    float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
    float T = draw_data->DisplayPos.y;
    float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
    ShaderData shaderData = { {
        { 2.0f / (R - L), 0.0f, 0.0f, 0.0f },
        { 0.0f, 2.0f / (T - B), 0.0f, 0.0f },
        { 0.0f, 0.0f, -1.0f, 0.0f },
        { (R + L) / (L - R), (T + B) / (B - T), 0.0f, 1.0f },
    } };

    uniformBuffer.SetSubData(0, sizeof(ShaderData), &shaderData);

    // Will project scissor/clipping rectangles into framebuffer space
    ImVec2 clip_off = draw_data->DisplayPos; // (0,0) unless using multi-viewports
    ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

    // Render command lists
    for (int n = 0; n < draw_data->CmdListsCount; n++) {
        const ImDrawList* cmd_list = draw_data->CmdLists[n];

        if (n >= indexBuffers.size()) {
            indexBuffers.resize(n + 1);
            dawn::BufferDescriptor descriptor;
            descriptor.size = kMaxVertexCount * sizeof(ImDrawIdx);
            descriptor.usage = dawn::BufferUsage::CopyDst | dawn::BufferUsage::Index;
            indexBuffers[n] = device.CreateBuffer(&descriptor);
        }

        if (n >= vertexBuffers.size()) {
            vertexBuffers.resize(n + 1);
            dawn::BufferDescriptor descriptor;
            descriptor.size = kMaxVertexCount * sizeof(ImDrawVert);
            descriptor.usage = dawn::BufferUsage::CopyDst | dawn::BufferUsage::Vertex;
            vertexBuffers[n] = device.CreateBuffer(&descriptor);
        }

        // Upload vertex/index buffers
        vertexBuffers[n].SetSubData(0, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cmd_list->VtxBuffer.Data);
        indexBuffers[n].SetSubData(0, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cmd_list->IdxBuffer.Data);

        for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++) {
            const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
            if (pcmd->UserCallback != NULL) {
                if (pcmd->UserCallback != ImDrawCallback_ResetRenderState) {
                    pcmd->UserCallback(cmd_list, pcmd);
                }
            } else {
                // Project scissor/clipping rectangles into framebuffer space
                ImVec4 clip_rect;
                clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
                clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
                clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
                clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

                if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f) {

                    const uint64_t vertexBufferOffsets[] = { pcmd->VtxOffset * sizeof(ImDrawVert) };

                    pass.SetScissorRect((int)clip_rect.x, (int)(clip_rect.y), (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y));
                    pass.SetPipeline(pipeline);
                    pass.SetBindGroup(0, bindGroup, 0, nullptr);
                    pass.SetVertexBuffers(0, 1, &vertexBuffers[n], vertexBufferOffsets);
                    pass.SetIndexBuffer(indexBuffers[n], pcmd->IdxOffset * sizeof(ImDrawIdx));
                    pass.DrawIndexed(pcmd->ElemCount, 1, 0, 0, 0);
                }
            }
        }
    }
}
