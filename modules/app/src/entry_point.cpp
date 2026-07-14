#include "osiris/app/application.hpp"

#include <memory>

int main()
{
    const std::unique_ptr<osiris::app::Application> app{osiris::app::CreateApplication()};
    app->Run();
    return 0;
}
