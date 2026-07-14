#include "osiris/rhi/shader_compiler.hpp"

#include "osiris/core/log.hpp"

#include <slang-com-ptr.h>
#include <slang.h>

#include <string>

namespace osiris::rhi {

struct ShaderCompiler::Impl {
    Slang::ComPtr<slang::IGlobalSession> global_session;
};

ShaderCompiler::ShaderCompiler()
    : m_impl(std::make_unique<Impl>())
{
    slang::createGlobalSession(m_impl->global_session.writeRef());
}

ShaderCompiler::~ShaderCompiler() = default;

std::vector<std::byte> ShaderCompiler::Compile(std::string_view source, std::string_view entry_point)
{
    slang::TargetDesc target = {};
    target.format = SLANG_DXIL;
    target.profile = m_impl->global_session->findProfile("sm_6_0");

    slang::SessionDesc session_desc = {};
    session_desc.targets = &target;
    session_desc.targetCount = 1;

    Slang::ComPtr<slang::ISession> session;

    const SlangResult result = m_impl->global_session->createSession(session_desc, session.writeRef());
    if (SLANG_FAILED(result) || !session)
    {
        osiris::core::Error("Failed to create Slang session.");
        return {};
    }

    const std::string src(source);

    Slang::ComPtr<slang::IBlob> diagnostics;
    slang::IModule* module = session->loadModuleFromSourceString("osiris_shader", "osiris_shader.slang", src.c_str(), diagnostics.writeRef());

    if (module == nullptr)
    {
        if (diagnostics)
        {
            osiris::core::Error(static_cast<const char*>(diagnostics->getBufferPointer()));
        }

        return {};
    }

    const std::string entry(entry_point);
    Slang::ComPtr<slang::IEntryPoint>entry_point_component;
    module->findEntryPointByName(entry.c_str(), entry_point_component.writeRef());

    if (!entry_point_component)
    {
        osiris::core::Error(std::string("Shader entry point not found: ") + entry);
        return {};
    }

    slang::IComponentType* components[] = { module, entry_point_component.get() };
    Slang::ComPtr<slang::IComponentType> program;

    const SlangResult compose_result = session->createCompositeComponentType(components, 2, program.writeRef(), diagnostics.writeRef());
    if (SLANG_FAILED(compose_result) || !program)
    {
        if (diagnostics)
        {
            osiris::core::Error(static_cast<const char*>(diagnostics->getBufferPointer()));
        }

        return {};
    }

    Slang::ComPtr<slang::IComponentType> linked;
    program->link(linked.writeRef(), diagnostics.writeRef());

    Slang::ComPtr<slang::IBlob> code;
    if (SLANG_FAILED(linked->getEntryPointCode(0, 0, code.writeRef())))
    {
        if (diagnostics)
        {
            osiris::core::Error(static_cast<const char*>(code->getBufferPointer()));
        }

        return {};
    }

    const auto* bytes = static_cast<const std::byte*>(code->getBufferPointer());
    return std::vector<std::byte>(bytes, bytes + code->getBufferSize());
}

} // namespace osiris::rhi
