//
// RT64
//

#include "rt64_librashader.h"

#include <vector>

#include "plume_d3d12.h"

#define LIBRA_RUNTIME_D3D12
#define LIBRA_RUNTIME_VULKAN
#include "../../include/librashader.h"
#include "../../include/librashader_ld.h"


namespace RT64 {
    // Librashader
    libra_instance_t libra;
    libra_shader_preset_t preset = nullptr;
    libra_d3d12_filter_chain_t filterChain = nullptr;

    std::vector<LibraRuntimeParam> currentRuntimeParams;

    std::string currentShaderPath;

    Librashader::Librashader() {
        libra = librashader_load_instance();
    }

    Librashader::~Librashader() { 
        // todo: cleanup of libra instance?
    }

    std::string Librashader::currentShader() {
        return currentShaderPath;
    }

    std::vector<LibraRuntimeParam> Librashader::getRuntimeParams() {
        return currentRuntimeParams;
    }

    void Librashader::updateRuntimeParam(LibraRuntimeParam parameter) {
        // todo: api specific routing
        if (filterChain) {
            libra.d3d12_filter_chain_set_param(&filterChain, parameter.name.c_str(), parameter.current_value);
        }
    }

    void Librashader::postprocess(const Librashader::LibraFrameParams &lp) {
        // librashader requires commandlist to be wrapped for current phase
        lp.commandList->end();

        const RenderCommandList *constCmd = lp.worker->commandList.get();
		lp.worker->commandQueue->executeCommandLists(&constCmd, 1, nullptr, 0, nullptr, 0, lp.worker->commandFence.get());
		lp.worker->wait();

        // new list for menu to avoid libra corruption
		lp.commandList->begin();

		lp.commandList->barriers(RenderBarrierStage::GRAPHICS, RenderTextureBarrier(lp.intermediateTexture, RenderTextureLayout::SHADER_READ));
		lp.commandList->barriers(RenderBarrierStage::GRAPHICS, RenderTextureBarrier(lp.swapchainTexture, RenderTextureLayout::COLOR_WRITE));
		lp.commandList->setFramebuffer(lp.swapchainFramebuffer);
		lp.commandList->clearColor();

		// librashader hookup
		auto* d3d12CmdList = static_cast<plume::D3D12CommandList*>(lp.commandList)->d3d;
		auto* d3d12Input = static_cast<plume::D3D12Texture*>(lp.intermediateTexture)->d3d;
		auto* d3d12Output = static_cast<plume::D3D12Texture*>(lp.swapchainTexture)->d3d;

		libra_image_d3d12_handle_t input_handle = { d3d12Input };
		libra_image_d3d12_handle_t output_handle = { d3d12Output };
		libra_image_d3d12_t input = {};
		libra_image_d3d12_t output = {};
		input.image_type = LIBRA_D3D12_IMAGE_TYPE_RESOURCE;
		output.image_type = LIBRA_D3D12_IMAGE_TYPE_RESOURCE;
		input.handle = input_handle;
		output.handle = output_handle;

		libra_error_t frameErr = libra.d3d12_filter_chain_frame(&filterChain, d3d12CmdList, lp.frameCount,
																input, output, NULL, NULL, NULL);
    }

    bool Librashader::ready() {
        return filterChain; 
    }

    void Librashader::reset() {
        if (filterChain) {
            libra.d3d12_filter_chain_free(&filterChain);
            filterChain = nullptr;
        }

        if (preset) {
            libra.preset_free(&preset);
            preset = nullptr;
        }

        currentRuntimeParams.clear();
    }

    bool Librashader::setup(RenderDevice *device, std::string path) {
        reset();

        libra_error_t err = libra.preset_create(path.c_str(), &preset);
        // todo: error checks for preset create and get runtime params
        //libra.preset_print(&preset);

        // Build out parameter vector
        currentRuntimeParams.clear();
        libra_preset_param_list_t preset_parameters;
        err = libra.preset_get_runtime_params(&preset, &preset_parameters);

        for (int i = 0; i < preset_parameters.length; i++) {
            libra_preset_param_t param = preset_parameters.parameters[i];
            currentRuntimeParams.push_back({
                param.name,
                param.description,
                param.initial, // keep track of this in case we want to reset to defaults
                param.minimum,
                param.maximum,
                param.step,
                param.initial // current_value = initial during init
            });
        }
        libra.preset_free_runtime_params(preset_parameters);

        auto* d3d12Device = static_cast<plume::D3D12Device*>(device);

		const filter_chain_d3d12_opt_t filterOptions = {
		    LIBRASHADER_CURRENT_VERSION,
		    false, // Force use of hlsl
		    false, // Force disable mipmaps
		    true   // Disable cache for UWP, it blows up the driver
	    };

        err = libra.d3d12_filter_chain_create(&preset, d3d12Device->d3d,
                                                            &filterOptions,
                                                            &filterChain);

        if (!err)
            currentShaderPath = path;

        return !err;
    }

}