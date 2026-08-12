#include "LineManager.h"

LineManager::LineManager() : line(nullptr) {}
LineManager::~LineManager() { if (line) line->Release(); }

bool LineManager::Initialize(IDirect3DDevice9* device) {
    HRESULT hr = D3DXCreateLine(device, &line);
    return SUCCEEDED(hr);
}

void LineManager::DrawLine(D3DXVECTOR2 p1, D3DXVECTOR2 p2, float thickness, D3DCOLOR color) {
    if (!line) return;
    D3DXVECTOR2 verts[2] = { p1, p2 };
    line->SetWidth(thickness);
    line->Begin();
    line->Draw(verts, 2, color);
    line->End();
}

void LineManager::DrawFilledRect(RECT rect, D3DCOLOR color) {
    if (!line) return;
    float height = (float)(rect.bottom - rect.top);
    D3DXVECTOR2 verts[2];
    verts[0] = D3DXVECTOR2((float)rect.left, rect.top + (height / 2.0f));
    verts[1] = D3DXVECTOR2((float)rect.right, rect.top + (height / 2.0f));

    line->SetWidth(height);
    line->Begin();
    line->Draw(verts, 2, color);
    line->End();
}

void LineManager::OnLostDevice() { if (line) line->OnLostDevice(); }
void LineManager::OnResetDevice() { if (line) line->OnResetDevice(); }