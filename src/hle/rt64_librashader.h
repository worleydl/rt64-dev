//
// RT64
//

#pragma once

#include "common/rt64_plume.h"
#include "render/rt64_render_worker.h"

namespace RT64 {
    struct Librashader {
        struct LibraParams {
            RenderCommandList *commandList = nullptr;
            RenderFramebuffer* swapchainFramebuffer = nullptr;
            RenderTexture *intermediateTexture = nullptr;
            RenderTexture *swapchainTexture = nullptr;
            RenderWorker *worker = nullptr;
            size_t frameCount;
        };

        Librashader();
        ~Librashader();
        std::string currentShader();
        bool ready();
        void reset();
        bool setup(RenderDevice* device, std::string path);
        void postprocess(const LibraParams &p);
    };
}