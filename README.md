# Osiris Engine — Documentation

Модульный игровой движок на **C++26** с опорой на **статическую рефлексию (P2996)**.
Цель — собрать лучшие идеи из современных движков (Hazel, Godot, Unreal, O3DE, bgfx/Diligent)
и построить чистую, модульную, data-driven архитектуру с несколькими RHI-бэкендами
и редактором, управляемым рефлексией.

| # | Документ | О чём |
|---|----------|-------|
| 00 | [Vision & Principles](architecture/00-vision-and-principles.md) | Зачем, за счёт чего, ключевые принципы, нефункциональные требования |
| 01 | [Modularity](architecture/01-modularity.md) | Слои, модули, границы, зависимости, plugin-система |
| 02 | [Reflection (C++26)](architecture/02-reflection-cpp26.md) | P2996, что даёт, фасад рефлексии, кодоген как fallback |
| 03 | [RHI](architecture/03-rhi.md) | Абстракция GPU: Vulkan / D3D12 / Metal, bindless, уровень абстракции |
| 04 | [Render Graph](architecture/04-render-graph.md) | Frame graph, авто-барьеры, планирование пассов |
| 05 | [Editor & Node Editor](architecture/05-editor-and-node-editor.md) | Редактор поверх runtime, reflection-inspector, node-графы |
| 06 | [Open Decisions](architecture/06-open-decisions.md) | Развилки: статусы, варианты, рекомендации |
| 07 | [Application & Sandbox](architecture/07-application-and-sandbox.md) | Каркас приложения, Sandbox/Editor/Player, layer stack (по мотивам Hazel) |
| 08 | [Module System & ABI](architecture/08-module-system-and-abi.md) | **Заменяемые DLL-подсистемы** (Audio/Render/…), ABI-границы, общий core, ModuleManager |

## Глоссарий кодовых имён

- `osiris::core` — базовые примитивы, аллокаторы, лог, job-system
- `osiris::rhi` — Render Hardware Interface
- `osiris::render` — высокоуровневый рендерер + render graph
- `osiris::scene` — сцена, ECS, компоненты
- `osiris::assets` — импорт/кук/загрузка ассетов
- `osiris::reflect` — фасад рефлексии
- `osiris::editor` — редактор (не линкуется в шиппинг-билд)
