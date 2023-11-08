#include "FullscreenQuad.h"
#include "Shader.h"
#include "misc.h"

FullscreenQuad::FullscreenQuad(ID3D11Device* device)
{
    create_vs_from_cso(device, "Shader\\FullscreenQuadVS.cso", embeddedVertexShader.ReleaseAndGetAddressOf(),
        nullptr, nullptr, 0);
    create_ps_from_cso(device, "Shader\\FullscreenQuadPS.cso", embeddedPixelShader.ReleaseAndGetAddressOf());

    // シーン用バッファ
    D3D11_BUFFER_DESC bufferDesc;
    ::memset(&bufferDesc, 0, sizeof(bufferDesc));
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;
    bufferDesc.StructureByteStride = 0;
    HRESULT hr = { S_OK };
    // ジッタードリフト用定数バッファ
    {
        bufferDesc.ByteWidth = sizeof(JitterDriftConstantBuffer);
        hr = device->CreateBuffer(&bufferDesc, nullptr, jitterDriftConstantBuffer.GetAddressOf());
        _ASSERT_EXPR(SUCCEEDED(hr), HRTrace(hr));
    }
}
void FullscreenQuad::blit(ID3D11DeviceContext* immediate_context,
    ID3D11ShaderResourceView** shader_resource_view, uint32_t start_slot, uint32_t num_views,
    ID3D11PixelShader* replaced_pixel_shader)
{
    immediate_context->IASetVertexBuffers(0, 0, nullptr, nullptr, nullptr);
    immediate_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    immediate_context->IASetInputLayout(nullptr);

    immediate_context->VSSetShader(embeddedVertexShader.Get(), 0, 0);
    replaced_pixel_shader ? immediate_context->PSSetShader(replaced_pixel_shader, 0, 0) :
        immediate_context->PSSetShader(embeddedPixelShader.Get(), 0, 0);

    immediate_context->PSSetShaderResources(start_slot, num_views, shader_resource_view);

    immediate_context->Draw(4, 0);
}

void FullscreenQuad::JitterDrift(ID3D11DeviceContext* immediate_context,
    ID3D11ShaderResourceView** shader_resource_view, uint32_t start_slot, uint32_t num_views,
    JitterDriftData jitterDriftData, ID3D11PixelShader* replaced_pixel_shader)
{
    JitterDriftConstantBuffer cbJitterDrift;
    cbJitterDrift.jitterDriftStrength = jitterDriftData.jitterStrength;
    cbJitterDrift.time = jitterDriftData.time;
    immediate_context->VSSetConstantBuffers(start_slot, num_views, jitterDriftConstantBuffer.GetAddressOf());
    immediate_context->PSSetConstantBuffers(start_slot, num_views, jitterDriftConstantBuffer.GetAddressOf());
    immediate_context->UpdateSubresource(jitterDriftConstantBuffer.Get(), 0, 0, &cbJitterDrift, 0, 0);

    blit(immediate_context, shader_resource_view, start_slot, num_views, replaced_pixel_shader);
}