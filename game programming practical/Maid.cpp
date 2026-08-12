#include "Maid.h"

void Maid::Init(IDirect3DDevice9* device) {
    d3dDevice = device;
}

void Maid::GiveTask(IUnknown* resource) {
    if (resource) {
        cleanupTasks.push_back(resource);
    }
}

void Maid::DoCleaning() {
    // iterate backwards to safely release memory
    for (int i = cleanupTasks.size() - 1; i >= 0; i--) {
        if (cleanupTasks[i]) {
            cleanupTasks[i]->Release();
        }
    }
    cleanupTasks.clear();
}