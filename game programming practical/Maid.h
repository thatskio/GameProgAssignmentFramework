#pragma once

#include <vector>
#include <d3d9.h>
#include <d3dx9.h>

class Maid {
private:
    IDirect3DDevice9* d3dDevice;
    std::vector<IUnknown*> cleanupTasks; // we use IUnknown so that it collects every d3dx data type

public:
    void Init(IDirect3DDevice9* device);
    IDirect3DDevice9* GetDevice() const { return d3dDevice; }

    void GiveTask(IUnknown* resource);
    void DoCleaning();
};