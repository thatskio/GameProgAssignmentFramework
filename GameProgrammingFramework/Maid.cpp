#include "Maid.h"

void Maid::Init(IDirect3DDevice9* device) {
    d3dDevice = device;
}

void Maid::GiveTask(IUnknown* task) {
    if (task) {
        cleanupTasks.push_back(task);
    }
}

void Maid::DoCleaning() {
    //Iterate backwards to safely release memory
    for (int i = cleanupTasks.size() - 1; i >= 0; i--) {
        if (cleanupTasks[i]) {
            cleanupTasks[i]->Release();
        }
    }
    cleanupTasks.clear();
}

Maid::~Maid() {
    DoCleaning();
}
