#pragma once

#include "../render/rt64_shader_common.h"

// Ugly export so I can disable game input since console is using pad for both inspector/game
extern bool showing_inspector;

// Shaders that have issues with uber on UWP so we precompile
// If you notice other issues, dump the pixel shader using the inspector, create a new entry below and add it to rt64_application
// TODO: Investigate codepath of these params to see if there's anything to try out
#ifdef UWP_SHADERS
namespace RT64 {

	// Two layer grass
	ShaderDescription desc_multitexture = {
		interop::ColorCombiner{ 4230425604U, 520918015U },
		interop::OtherMode{ 3356565624U, 1584128U },
		interop::RenderFlags{ .value = 1217265678U }
	};

	// North Mountains 
	ShaderDescription desc_mountain = {
		interop::ColorCombiner{ 4230446596U, 520943615U },
		interop::OtherMode{ 3356565624U, 1584128U },
		interop::RenderFlags{ .value = 1217792014U }
	};

	// Ice 
	ShaderDescription desc_ice = {
		interop::ColorCombiner{ 4230425696U, 890540927U },
		interop::OtherMode{ 202918360U, 1584128U },
		interop::RenderFlags{.value = 1217265674U }
	};

	// Iris (may need additional for iris contents, this keeps edge round)
	ShaderDescription desc_iris = {
		interop::ColorCombiner{4229076996U, 1058406399U},
		interop::OtherMode{202916296U, 9972799U},
		interop::RenderFlags{.value = 2290745358}
	};

	ShaderDescription desc_iris_contents = {
		interop::ColorCombiner{4229076996U, 1058406399U},
		interop::OtherMode{202916432U, 9972799U},
		interop::RenderFlags{.value = 2290745358}
	};

	// Dirt path blend
	ShaderDescription desc_dirtpath = {
		interop::ColorCombiner{4230387716U, 521469951U},
		interop::OtherMode{202916464U, 10038335U},
		interop::RenderFlags{.value = 2559442958}
	};

	// Fence
	ShaderDescription desc_fence = {
		interop::ColorCombiner{4230387716U, 521469951U},
		interop::OtherMode{202916432U, 10038335U},
		interop::RenderFlags{.value = 2559442954}
	};

	/* Ice chandelier */
	/*
	RenderParams rp; rp.omL = 3356510680U;
	rp.omH = 1584128U;
	rp.ccL = 4229398112U;
	rp.ccH = 905869176U;
	rp.flags = 1217003534;
	*/

	/*Working wall*/
	/*
	RenderParams rp; rp.omL = 3356565624U;
	rp.omH = 1584128U;
	rp.ccL = 4229999620U;
	rp.ccH = 4279211007U;
	rp.flags = 1284497422;;
	*/
}
#endif

