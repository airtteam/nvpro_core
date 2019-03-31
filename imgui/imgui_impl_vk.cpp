#include "imgui.h"
#include "imgui_impl_vk.h"
#include <assert.h>

namespace
{
  const unsigned char vert_spv_data[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x2F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // #version 450 core
    0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4C, 0x53, 0x4C, //
    0x2E, 0x73, 0x74, 0x64, 0x2E, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, // layout(location = 0) in vec2 aPos;
    0x01, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x69, 0x6E, // layout(location = 1) in vec2 aUV;
    0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // layout(location = 2) in vec4 aColor;
    0x26, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, //
    0x04, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00, // layout(push_constant) uniform uPushConstant {
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x05, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x6F, 0x6C, 0x6F, //   mat4 ProjectionMatrix;
    0x72, 0x00, 0x00, 0x00, 0x06, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x55, 0x56, 0x00, 0x00, // } Pc;
    0x05, 0x00, 0x03, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x4F, 0x75, 0x74, 0x00, 0x05, 0x00, 0x04, 0x00, 0x0F, 0x00, 0x00, 0x00, //
    0x61, 0x43, 0x6F, 0x6C, 0x6F, 0x72, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x15, 0x00, 0x00, 0x00, 0x61, 0x55, 0x56, 0x00, // layout(location = 0) out struct {
    0x05, 0x00, 0x06, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x67, 0x6C, 0x5F, 0x50, 0x65, 0x72, 0x56, 0x65, 0x72, 0x74, 0x65, 0x78, //   vec4 Color;
    0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x06, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x67, 0x6C, 0x5F, 0x50, //   vec2 UV;
    0x6F, 0x73, 0x69, 0x74, 0x69, 0x6F, 0x6E, 0x00, 0x06, 0x00, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, // } Out;
    0x67, 0x6C, 0x5F, 0x50, 0x6F, 0x69, 0x6E, 0x74, 0x53, 0x69, 0x7A, 0x65, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x07, 0x00, //
    0x1C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x67, 0x6C, 0x5F, 0x43, 0x6C, 0x69, 0x70, 0x44, 0x69, 0x73, 0x74, 0x61, // void main()
    0x6E, 0x63, 0x65, 0x00, 0x06, 0x00, 0x07, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x67, 0x6C, 0x5F, 0x43, // {
    0x75, 0x6C, 0x6C, 0x44, 0x69, 0x73, 0x74, 0x61, 0x6E, 0x63, 0x65, 0x00, 0x05, 0x00, 0x03, 0x00, 0x1E, 0x00, 0x00, 0x00, //   Out.Color = aColor;
    0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x06, 0x00, 0x20, 0x00, 0x00, 0x00, 0x75, 0x50, 0x75, 0x73, 0x68, 0x43, 0x6F, 0x6E, //   Out.UV = aUV;
    0x73, 0x74, 0x61, 0x6E, 0x74, 0x00, 0x00, 0x00, 0x06, 0x00, 0x08, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //   gl_Position = Pc.ProjectionMatrix * vec4(aPos, 0, 1);
    0x50, 0x72, 0x6F, 0x6A, 0x65, 0x63, 0x74, 0x69, 0x6F, 0x6E, 0x4D, 0x61, 0x74, 0x72, 0x69, 0x78, 0x00, 0x00, 0x00, 0x00, // }
    0x05, 0x00, 0x03, 0x00, 0x22, 0x00, 0x00, 0x00, 0x50, 0x63, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x26, 0x00, 0x00, 0x00,
    0x61, 0x50, 0x6F, 0x73, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x47, 0x00, 0x04, 0x00, 0x15, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x48, 0x00, 0x04, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x23, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x48, 0x00, 0x05, 0x00, 0x20, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x10, 0x00, 0x00, 0x00, 0x47, 0x00, 0x03, 0x00, 0x20, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00,
    0x26, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00,
    0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x17, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x04, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x2B, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00,
    0x0E, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00, 0x0E, 0x00, 0x00, 0x00,
    0x0F, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x11, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00,
    0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x17, 0x00, 0x00, 0x00,
    0x03, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x1C, 0x00, 0x04, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x06, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00,
    0x1D, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x18, 0x00, 0x04, 0x00, 0x1F, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x03, 0x00, 0x20, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x21, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00,
    0x21, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x23, 0x00, 0x00, 0x00,
    0x09, 0x00, 0x00, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00,
    0x01, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x2B, 0x00, 0x04, 0x00, 0x06, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0x3F, 0x36, 0x00, 0x05, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x02, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,
    0x3E, 0x00, 0x03, 0x00, 0x12, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00,
    0x16, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x17, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00,
    0x0B, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x03, 0x00, 0x18, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x05, 0x00, 0x23, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,
    0x3D, 0x00, 0x04, 0x00, 0x1F, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x24, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x04, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x26, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x2A, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x51, 0x00, 0x05, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x2B, 0x00, 0x00, 0x00, 0x27, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x00, 0x07, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x2C, 0x00, 0x00, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x00, 0x00, 0x28, 0x00, 0x00, 0x00, 0x29, 0x00, 0x00, 0x00,
    0x91, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x25, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00,
    0x41, 0x00, 0x05, 0x00, 0x11, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,
    0x3E, 0x00, 0x03, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x2D, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00,
  };
  const unsigned char frag_spv_data[] = {
    0x03, 0x02, 0x23, 0x07, 0x00, 0x00, 0x01, 0x00, 0x04, 0x00, 0x08, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // #version 450 core
    0x11, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x06, 0x00, 0x01, 0x00, 0x00, 0x00, 0x47, 0x4C, 0x53, 0x4C, //
    0x2E, 0x73, 0x74, 0x64, 0x2E, 0x34, 0x35, 0x30, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, // layout(location = 0) out vec4 OutCol;
    0x01, 0x00, 0x00, 0x00, 0x0F, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x69, 0x6E, //
    0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x10, 0x00, 0x03, 0x00, 0x04, 0x00, 0x00, 0x00, // layout(set = 0, binding = 0) uniform sampler2D sTexture;
    0x07, 0x00, 0x00, 0x00, 0x03, 0x00, 0x03, 0x00, 0x02, 0x00, 0x00, 0x00, 0xC2, 0x01, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, //
    0x04, 0x00, 0x00, 0x00, 0x6D, 0x61, 0x69, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, // layout(location = 0) in struct {
    0x4F, 0x75, 0x74, 0x43, 0x6F, 0x6C, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //   vec4 Color;
    0x06, 0x00, 0x05, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x43, 0x6F, 0x6C, 0x6F, 0x72, 0x00, 0x00, 0x00, //   vec2 UV;
    0x06, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x55, 0x56, 0x00, 0x00, 0x05, 0x00, 0x03, 0x00, // } In;
    0x0D, 0x00, 0x00, 0x00, 0x49, 0x6E, 0x00, 0x00, 0x05, 0x00, 0x05, 0x00, 0x16, 0x00, 0x00, 0x00, 0x73, 0x54, 0x65, 0x78, //
    0x74, 0x75, 0x72, 0x65, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x09, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, // void main()
    0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // {
    0x47, 0x00, 0x04, 0x00, 0x16, 0x00, 0x00, 0x00, 0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x47, 0x00, 0x04, 0x00, //   OutCol = In.Color * texture(sTexture, In.UV);
    0x16, 0x00, 0x00, 0x00, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x02, 0x00, 0x02, 0x00, 0x00, 0x00, // }
    0x21, 0x00, 0x03, 0x00, 0x03, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x16, 0x00, 0x03, 0x00, 0x06, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x08, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00,
    0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0x17, 0x00, 0x04, 0x00, 0x0A, 0x00, 0x00, 0x00,
    0x06, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x04, 0x00, 0x0B, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x0A, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0B, 0x00, 0x00, 0x00,
    0x3B, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x15, 0x00, 0x04, 0x00,
    0x0E, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x04, 0x00, 0x0E, 0x00, 0x00, 0x00,
    0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x10, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x07, 0x00, 0x00, 0x00, 0x19, 0x00, 0x09, 0x00, 0x13, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x1B, 0x00, 0x03, 0x00, 0x14, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x20, 0x00, 0x04, 0x00, 0x15, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x3B, 0x00, 0x04, 0x00, 0x15, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x2B, 0x00, 0x04, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00,
    0x20, 0x00, 0x04, 0x00, 0x19, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x36, 0x00, 0x05, 0x00,
    0x02, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x00, 0x00, 0xF8, 0x00, 0x02, 0x00,
    0x05, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00, 0x10, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00,
    0x0F, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x04, 0x00, 0x07, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x11, 0x00, 0x00, 0x00,
    0x3D, 0x00, 0x04, 0x00, 0x14, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x16, 0x00, 0x00, 0x00, 0x41, 0x00, 0x05, 0x00,
    0x19, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x0D, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x3D, 0x00, 0x04, 0x00,
    0x0A, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x1A, 0x00, 0x00, 0x00, 0x57, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x1C, 0x00, 0x00, 0x00, 0x17, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x00, 0x00, 0x85, 0x00, 0x05, 0x00, 0x07, 0x00, 0x00, 0x00,
    0x1D, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x03, 0x00, 0x09, 0x00, 0x00, 0x00,
    0x1D, 0x00, 0x00, 0x00, 0xFD, 0x00, 0x01, 0x00, 0x38, 0x00, 0x01, 0x00,
  };

  nv_helpers_vk::DeviceUtils s_utils = {};
  nv_helpers_vk::PhysicalInfo s_physical = {};

  VkImage s_fontAtlas = VK_NULL_HANDLE;
  VkImageView s_fontAtlasView = VK_NULL_HANDLE;
  VkSampler s_fontAtlasSampler = VK_NULL_HANDLE;
  VkDeviceMemory s_fontAtlasMem = VK_NULL_HANDLE;
  VkPipeline s_pipeline = VK_NULL_HANDLE;
  nv_helpers_vk::DescriptorPipelineContainer<1> s_pipelineSetup = {};

  VkBuffer s_ibo = VK_NULL_HANDLE;
  VkDeviceMemory s_iboMem = VK_NULL_HANDLE;
  VkBuffer s_vbo = VK_NULL_HANDLE;
  VkDeviceMemory s_vboMem = VK_NULL_HANDLE;
}

void ImGui::InitVK(const nv_helpers_vk::DeviceUtils &utils, const nv_helpers_vk::PhysicalInfo &physical, VkRenderPass pass, VkQueue queue, uint32_t queueFamilyIndex)
{
  using namespace nv_helpers_vk;

  assert(utils.m_device != VK_NULL_HANDLE);

  // Destroy any existing resources
  ShutdownVK();

  s_utils = utils;
  s_physical = physical;

  // Load font atlas data
  int fontAtlasWidth = 0;
  int fontAtlasHeight = 0;
  uint8_t *fontAtlasData = nullptr;
  GetIO().Fonts->GetTexDataAsRGBA32(&fontAtlasData, &fontAtlasWidth, &fontAtlasHeight);

  assert(fontAtlasWidth != 0 && fontAtlasHeight != 0 && fontAtlasData != nullptr);

  VkResult res = VK_RESULT_MAX_ENUM;

  // Create and allocate font atlas image
  { VkImageCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO };
    createInfo.imageType = VK_IMAGE_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.extent.width = fontAtlasWidth;
    createInfo.extent.height = fontAtlasHeight;
    createInfo.extent.depth = 1;
    createInfo.mipLevels = 1;
    createInfo.arrayLayers = 1;
    createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
    createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
    createInfo.usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;
    createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
    createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

    res = vkCreateImage(utils.m_device, &createInfo, utils.m_allocator, &s_fontAtlas);
    assert(res == VK_SUCCESS);
  }

  { VkMemoryRequirements reqs = {};
    vkGetImageMemoryRequirements(utils.m_device, s_fontAtlas, &reqs);

    VkMemoryAllocateInfo allocInfo = { VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
    allocInfo.allocationSize = reqs.size;
    allocInfo.memoryTypeIndex = PhysicalDeviceMemoryProperties_getMemoryTypeIndex(physical.memoryProperties, reqs, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

    res = vkAllocateMemory(utils.m_device, &allocInfo, utils.m_allocator, &s_fontAtlasMem);
    assert(res == VK_SUCCESS);

    res = vkBindImageMemory(utils.m_device, s_fontAtlas, s_fontAtlasMem, 0);
    assert(res == VK_SUCCESS);
  }

  // Create image view for font atlas image
  { VkImageViewCreateInfo createInfo = { VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO };
    createInfo.image = s_fontAtlas;
    createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    createInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    createInfo.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    res = vkCreateImageView(utils.m_device, &createInfo, utils.m_allocator, &s_fontAtlasView);
    assert(res == VK_SUCCESS);
  }

  // Copy font atlas data from host to device memory
  { BasicStagingBuffer staging;
    staging.init(utils.m_device, &physical.memoryProperties);

    staging.enqueue(s_fontAtlas,
      { 0, 0, 0 }, { uint32_t(fontAtlasWidth), uint32_t(fontAtlasHeight), 1 },
      { VK_IMAGE_ASPECT_COLOR_BIT, 0, 0, 1 },
      fontAtlasWidth * fontAtlasHeight * 4, fontAtlasData);

    VkCommandPool pool = VK_NULL_HANDLE;
    { VkCommandPoolCreateInfo createInfo { VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO };
      createInfo.queueFamilyIndex = queueFamilyIndex;

      res = vkCreateCommandPool(utils.m_device, &createInfo, utils.m_allocator, &pool);
      assert(res == VK_SUCCESS);
    }

    VkCommandBuffer cmd = VK_NULL_HANDLE;
    { VkCommandBufferAllocateInfo allocInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO };
      allocInfo.commandPool = pool;
      allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
      allocInfo.commandBufferCount = 1;

      res = vkAllocateCommandBuffers(utils.m_device, &allocInfo, &cmd);
      assert(res == VK_SUCCESS);
    }

    { VkCommandBufferBeginInfo beginInfo { VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO };
      beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

      res = vkBeginCommandBuffer(cmd, &beginInfo);
      assert(res == VK_SUCCESS);
    }

    VkImageMemoryBarrier transition { VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER };
    transition.srcAccessMask = 0;
    transition.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    transition.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    transition.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    transition.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    transition.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    transition.image = s_fontAtlas;
    transition.subresourceRange = { VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1 };

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &transition);

    staging.flush(cmd);

    transition.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    transition.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    transition.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    transition.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

    vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0, nullptr, 1, &transition);

    res = vkEndCommandBuffer(cmd);
    assert(res == VK_SUCCESS);

    { VkSubmitInfo submit { VK_STRUCTURE_TYPE_SUBMIT_INFO };
      submit.commandBufferCount = 1;
      submit.pCommandBuffers = &cmd;

      res = vkQueueSubmit(queue, 1, &submit, VK_NULL_HANDLE);
      assert(res == VK_SUCCESS);
    }

    vkDeviceWaitIdle(utils.m_device); // Beautiful

    vkFreeCommandBuffers(utils.m_device, pool, 1, &cmd);
    vkDestroyCommandPool(utils.m_device, pool, utils.m_allocator);

    staging.deinit();
  }

  // Create sampler state for font atlas
  { VkSamplerCreateInfo createInfo { VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO };
    createInfo.magFilter = VK_FILTER_LINEAR;
    createInfo.minFilter = VK_FILTER_LINEAR;
    createInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    createInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    createInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    createInfo.minLod = FLT_MIN;
    createInfo.maxLod = FLT_MAX;
    
    res = vkCreateSampler(utils.m_device, &createInfo, utils.m_allocator, &s_fontAtlasSampler);
    assert(res == VK_SUCCESS);
  }

  // Allocate descriptors and pipeline layout for the imgui shaders
  {
    const VkPushConstantRange pcRange = {
      VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 16
    };
    const VkDescriptorPoolSize poolSize = {
      VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1
    };
    const VkDescriptorSetLayoutBinding bindings[] = {
      s_utils.makeDescriptorSetLayoutBinding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT)
    };

    s_pipelineSetup.init(s_utils.m_device, s_utils.m_allocator);

    s_pipelineSetup.descriptorSetLayout[0] = s_utils.createDescriptorSetLayout(NV_ARRAY_SIZE(bindings), bindings);
    s_pipelineSetup.pipelineLayouts[0] = s_utils.createPipelineLayout(NV_ARRAY_SIZE(s_pipelineSetup.descriptorSetLayout), s_pipelineSetup.descriptorSetLayout, 1, &pcRange);

    s_pipelineSetup.initPoolAndSets(0, 1, 1, &poolSize);

    const VkDescriptorImageInfo imageInfo { s_fontAtlasSampler, s_fontAtlasView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL };

    VkWriteDescriptorSet write { VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET };
    write.dstSet = s_pipelineSetup.getSets()[0];
    write.descriptorCount = 1;
    write.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    write.pImageInfo = &imageInfo;

    vkUpdateDescriptorSets(utils.m_device, 1, &write, 0, nullptr);
  }

  if (pass) {
    ReInitPipelinesVK(utils, pass);
  }
}

void ImGui::ReInitPipelinesVK(const nv_helpers_vk::DeviceUtils &utils, VkRenderPass pass)
{
  if (s_pipeline) {
    vkDestroyPipeline(s_utils.m_device, s_pipeline, s_utils.m_allocator);
    s_pipeline = VK_NULL_HANDLE;
  }
  // Create pipeline state
  {
    VkResult res;

    VkPipelineShaderStageCreateInfo vertStage{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    vertStage.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertStage.pName = "main";
    { VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    createInfo.codeSize = sizeof(vert_spv_data);
    createInfo.pCode = reinterpret_cast<const uint32_t *>(vert_spv_data);
    res = vkCreateShaderModule(s_utils.m_device, &createInfo, utils.m_allocator, &vertStage.module);
    assert(res == VK_SUCCESS);
    }

    VkPipelineShaderStageCreateInfo fragStage{ VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO };
    fragStage.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragStage.pName = "main";
    { VkShaderModuleCreateInfo createInfo{ VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO };
    createInfo.codeSize = sizeof(frag_spv_data);
    createInfo.pCode = reinterpret_cast<const uint32_t *>(frag_spv_data);
    res = vkCreateShaderModule(s_utils.m_device, &createInfo, utils.m_allocator, &fragStage.module);
    assert(res == VK_SUCCESS);
    }

    const VkVertexInputBindingDescription vertexBindings[] = {
      { 0, sizeof(ImDrawVert), VK_VERTEX_INPUT_RATE_VERTEX }
    };

    const VkVertexInputAttributeDescription vertexAttributes[] = {
      { 0, vertexBindings[0].binding, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, pos) },
      { 1, vertexBindings[0].binding, VK_FORMAT_R32G32_SFLOAT, offsetof(ImDrawVert, uv) },
      { 2, vertexBindings[0].binding, VK_FORMAT_R8G8B8A8_UNORM, offsetof(ImDrawVert, col) },
    };

    VkPipelineVertexInputStateCreateInfo viStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO };
    viStateInfo.vertexBindingDescriptionCount = NV_ARRAY_SIZE(vertexBindings);
    viStateInfo.pVertexBindingDescriptions = vertexBindings;
    viStateInfo.vertexAttributeDescriptionCount = NV_ARRAY_SIZE(vertexAttributes);
    viStateInfo.pVertexAttributeDescriptions = vertexAttributes;

    VkPipelineInputAssemblyStateCreateInfo iaStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO };
    iaStateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;

    VkPipelineViewportStateCreateInfo vpStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO };
    vpStateInfo.scissorCount = 1;
    vpStateInfo.viewportCount = 1;

    VkPipelineRasterizationStateCreateInfo rsStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO };
    rsStateInfo.depthClampEnable = VK_TRUE;
    rsStateInfo.polygonMode = VK_POLYGON_MODE_FILL;
    rsStateInfo.cullMode = VK_CULL_MODE_NONE;
    rsStateInfo.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rsStateInfo.lineWidth = 1.0f;

    VkPipelineColorBlendAttachmentState cbAttachmentState;
    cbAttachmentState.blendEnable = true;
    cbAttachmentState.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
    cbAttachmentState.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    cbAttachmentState.colorBlendOp = VK_BLEND_OP_ADD;
    cbAttachmentState.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
    cbAttachmentState.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
    cbAttachmentState.alphaBlendOp = VK_BLEND_OP_ADD;
    cbAttachmentState.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

    VkPipelineColorBlendStateCreateInfo cbStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO };
    cbStateInfo.attachmentCount = 1;
    cbStateInfo.pAttachments = &cbAttachmentState;

    VkPipelineDepthStencilStateCreateInfo dsStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO };
    dsStateInfo.depthTestEnable = false;
    dsStateInfo.depthWriteEnable = false;
    dsStateInfo.depthCompareOp = VK_COMPARE_OP_LESS;

    VkPipelineMultisampleStateCreateInfo msStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO };
    msStateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    msStateInfo.sampleShadingEnable = VK_FALSE;
    msStateInfo.minSampleShading = 1.0f;

    const VkDynamicState dynStates[] = {
      VK_DYNAMIC_STATE_VIEWPORT,
      VK_DYNAMIC_STATE_SCISSOR
    };

    VkPipelineDynamicStateCreateInfo dynStateInfo{ VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO };
    dynStateInfo.dynamicStateCount = NV_ARRAY_SIZE(dynStates);
    dynStateInfo.pDynamicStates = dynStates;

    const VkPipelineShaderStageCreateInfo stages[] = {
      vertStage, fragStage
    };

    VkGraphicsPipelineCreateInfo createInfo{ VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO };
    createInfo.stageCount = NV_ARRAY_SIZE(stages);
    createInfo.pStages = stages;
    createInfo.pVertexInputState = &viStateInfo;
    createInfo.pInputAssemblyState = &iaStateInfo;
    createInfo.pViewportState = &vpStateInfo;
    createInfo.pRasterizationState = &rsStateInfo;
    createInfo.pMultisampleState = &msStateInfo;
    createInfo.pDepthStencilState = &dsStateInfo;
    createInfo.pColorBlendState = &cbStateInfo;
    createInfo.pDynamicState = &dynStateInfo;
    createInfo.layout = s_pipelineSetup.getPipeLayout();
    createInfo.renderPass = pass;
    createInfo.subpass = 0;

    res = vkCreateGraphicsPipelines(utils.m_device, VK_NULL_HANDLE, 1, &createInfo, utils.m_allocator, &s_pipeline);
    assert(res == VK_SUCCESS);

    vkDestroyShaderModule(utils.m_device, vertStage.module, utils.m_allocator);
    vkDestroyShaderModule(utils.m_device, fragStage.module, utils.m_allocator);
  }
}

void ImGui::ShutdownVK()
{
  if (!s_utils.m_device)
    return;

  vkDestroyBuffer(s_utils.m_device, s_vbo, s_utils.m_allocator);
  s_vbo = VK_NULL_HANDLE;
  vkDestroyBuffer(s_utils.m_device, s_ibo, s_utils.m_allocator);
  s_ibo = VK_NULL_HANDLE;

  vkDestroyImage(s_utils.m_device, s_fontAtlas, s_utils.m_allocator);
  s_fontAtlas = VK_NULL_HANDLE;
  vkDestroyImageView(s_utils.m_device, s_fontAtlasView, s_utils.m_allocator);
  s_fontAtlasView = VK_NULL_HANDLE;

  vkDestroySampler(s_utils.m_device, s_fontAtlasSampler, s_utils.m_allocator);
  s_fontAtlasSampler = VK_NULL_HANDLE;

  vkFreeMemory(s_utils.m_device, s_vboMem, s_utils.m_allocator);
  s_vboMem = VK_NULL_HANDLE;
  vkFreeMemory(s_utils.m_device, s_iboMem, s_utils.m_allocator);
  s_iboMem = VK_NULL_HANDLE;

  vkFreeMemory(s_utils.m_device, s_fontAtlasMem, s_utils.m_allocator);
  s_fontAtlasMem = VK_NULL_HANDLE;

  vkDestroyPipeline(s_utils.m_device, s_pipeline, s_utils.m_allocator);
  s_pipeline = VK_NULL_HANDLE;

  s_pipelineSetup.deinitPools();
  s_pipelineSetup.deinitLayouts();

  s_utils.m_device = VK_NULL_HANDLE;
  s_utils.m_allocator = nullptr;
}

void ImGui::RenderDrawDataVK(VkCommandBuffer cmd, const ImDrawData* drawData)
{
  if (drawData == nullptr || drawData->TotalIdxCount == 0 || drawData->TotalVtxCount == 0)
    return;

  // Resize vertex and index buffers
  static uint32_t lastIboSize = 0;
  uint32_t iboSize = drawData->TotalIdxCount * sizeof(ImDrawIdx);
  if (lastIboSize < iboSize || s_ibo == VK_NULL_HANDLE) {
    if (s_ibo != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(s_utils.m_device);
      vkDestroyBuffer(s_utils.m_device, s_ibo, s_utils.m_allocator);
      vkFreeMemory(s_utils.m_device, s_iboMem, s_utils.m_allocator);
    }
    lastIboSize = uint32_t(exp2(ceil(log2(iboSize)))); // Round up to next power of two
    s_ibo = s_utils.createBuffer(lastIboSize, VK_BUFFER_USAGE_INDEX_BUFFER_BIT);
    s_utils.allocMemAndBindBuffer(s_ibo, s_physical.memoryProperties, s_iboMem, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  }
  static uint32_t lastVboSize = 0;
  uint32_t vboSize = drawData->TotalVtxCount * sizeof(ImDrawVert);
  if (lastVboSize < vboSize || s_vbo == VK_NULL_HANDLE) {
    if (s_vbo != VK_NULL_HANDLE) {
      vkDeviceWaitIdle(s_utils.m_device);
      vkDestroyBuffer(s_utils.m_device, s_vbo, s_utils.m_allocator);
      vkFreeMemory(s_utils.m_device, s_vboMem, s_utils.m_allocator);
    }
    lastVboSize = uint32_t(exp2(ceil(log2(vboSize)))); // Round up to next power of two
    s_vbo = s_utils.createBuffer(lastVboSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
    s_utils.allocMemAndBindBuffer(s_vbo, s_physical.memoryProperties, s_vboMem, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
  }

  // Upload data to vertex and index buffers
  { ImDrawIdx* mappedMem = nullptr;
    assert(s_iboMem != VK_NULL_HANDLE);
    vkMapMemory(s_utils.m_device, s_iboMem, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&mappedMem));
    for (int i = 0, offset = 0; i < drawData->CmdListsCount; i++) {
      const ImDrawList *drawList = drawData->CmdLists[i];
      std::memcpy(mappedMem + offset, drawList->IdxBuffer.Data, drawList->IdxBuffer.Size * sizeof(ImDrawIdx));
      offset += drawList->IdxBuffer.size();
    }
    vkUnmapMemory(s_utils.m_device, s_iboMem);
  }
  { ImDrawVert* mappedMem = nullptr;
    assert(s_vboMem != VK_NULL_HANDLE);
    vkMapMemory(s_utils.m_device, s_vboMem, 0, VK_WHOLE_SIZE, 0, reinterpret_cast<void **>(&mappedMem));
    for (int i = 0, offset = 0; i < drawData->CmdListsCount; i++) {
      const ImDrawList *drawList = drawData->CmdLists[i];
      std::memcpy(mappedMem + offset, drawList->VtxBuffer.Data, drawList->VtxBuffer.Size * sizeof(ImDrawVert));
      offset += drawList->VtxBuffer.size();
    }
    vkUnmapMemory(s_utils.m_device, s_vboMem);
  }

  // Setup render states
  const VkDeviceSize offset = 0;
  vkCmdBindIndexBuffer(cmd, s_ibo, offset, VK_INDEX_TYPE_UINT16);
  vkCmdBindVertexBuffers(cmd, 0, 1, &s_vbo, &offset);
  vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, s_pipeline);
  vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, s_pipelineSetup.getPipeLayout(), 0, 1, s_pipelineSetup.getSets(), 0, nullptr);

  const VkViewport viewport = {
    0.0f, 0.0f, static_cast<float>(GetIO().DisplaySize.x), static_cast<float>(GetIO().DisplaySize.y), 0.0f, 1.0f
  };
  vkCmdSetViewport(cmd, 0, 1, &viewport);

  // Setup ortographic projection matrix
  const float mvp[16] = {
    2.0f / GetIO().DisplaySize.x, 0.0f, 0.0f, 0.0f,
    0.0f, 2.0f / GetIO().DisplaySize.y, 0.0f, 0.0f,
    0.0f, 0.0f, 0.5f, 0.0f,
    -1.0f, -1.0f, 0.5f, 1.0f };

  vkCmdPushConstants(cmd, s_pipelineSetup.getPipeLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mvp), mvp);

  // Render command lists
  for (uint32_t i = 0, ioffset = 0, voffset = 0; i < uint32_t(drawData->CmdListsCount); ++i)
  {
    const ImDrawList *drawList = drawData->CmdLists[i];

    for (const ImDrawCmd &drawCmd : drawList->CmdBuffer)
    {
      if (drawCmd.UserCallback != nullptr)
      {
        drawCmd.UserCallback(drawList, &drawCmd);
        continue;
      }

      const VkRect2D rect = {
        VkOffset2D {
          static_cast<int32_t>(drawCmd.ClipRect.x),
          static_cast<int32_t>(drawCmd.ClipRect.y) },
        VkExtent2D {
          static_cast<uint32_t>(drawCmd.ClipRect.z - drawCmd.ClipRect.x),
          static_cast<uint32_t>(drawCmd.ClipRect.w - drawCmd.ClipRect.y) }
      };

      vkCmdSetScissor(cmd, 0, 1, &rect);

      vkCmdDrawIndexed(cmd, drawCmd.ElemCount, 1, ioffset, voffset, 0);

      ioffset += drawCmd.ElemCount;
    }

    voffset += drawList->VtxBuffer.Size;
  }
}
