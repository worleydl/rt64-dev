//
// RT64
//

#pragma once

#include "common/rt64_plume.h"
#include "render/rt64_render_worker.h"

namespace RT64 {
    // Based off libra preset param but also keeps track of current value alongside initial to support minimal reset to default
    struct LibraRuntimeParam {
        std::string name;
        std::string description;
        float initial;
        float min;
        float max;
        float step;
        float current_value;
        bool dirty;

        LibraRuntimeParam(std::string n, std::string d, float def, float min_, float max_, float step_)
            : name(std::move(n)),
            description(std::move(d)),
            initial(def),
            min(min_), max(max_),
            step(step_),
            current_value(def),
            dirty(false) {}
    };

    struct Librashader {
        struct LibraFrameParams {
            RenderCommandList *commandList = nullptr;
            RenderFramebuffer* swapchainFramebuffer = nullptr;
            RenderTexture *intermediateTexture = nullptr;
            RenderTexture *swapchainTexture = nullptr;
            RenderWorker *worker = nullptr;
            size_t frameCount;
        };

        struct LibraRuntimeState {
            std::vector<LibraRuntimeParam> params;
            bool dirty;
        };

        Librashader();
        ~Librashader();
        std::string getCurrentShader();
        std::vector<LibraRuntimeParam>& getRuntimeParams();
        void updateRuntimeParam(const LibraRuntimeParam param);
        bool ready();
        void reset();
        bool setup(RenderDevice* device, std::string path);
        void postprocess(const LibraFrameParams &p);
    };
}