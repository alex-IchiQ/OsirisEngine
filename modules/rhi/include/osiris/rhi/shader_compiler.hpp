#pragma once

#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

#include "osiris/rhi/rhi_api.hpp"

namespace osiris::rhi {

// Compiles Slang source to DXIL in-process. The shader stage comes from the entry
// point's [shader("...")] attribute in the source. Slang details are hidden behind a pimpl.
class OSIRIS_RHI_API ShaderCompiler
{
public:
    ShaderCompiler();
    ~ShaderCompiler();

    ShaderCompiler(const ShaderCompiler&) = delete;
    ShaderCompiler& operator=(const ShaderCompiler&) = delete;

    // Returns DXIL bytecode for the entry point, or an empty vector on failure.
    std::vector<std::byte> Compile(std::string_view source, std::string_view entry_point);

private:
    struct Impl;
    std::unique_ptr<Impl> m_impl;
};

} // namespace osiris::rhi
