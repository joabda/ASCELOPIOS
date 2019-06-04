#include "pluginmanager.h"

PluginManager::PluginManager()
{
    experimental::filesystem::path dir(TOOLS_FOLDER);
    experimental::filesystem::create_directory(dir);
    for (experimental::filesystem::directory_entry e : experimental::filesystem::directory_iterator(TOOLS_FOLDER))
    {
        experimental::filesystem::path p = e.path();
        if ((p.extension() == ".sh") || (p.extension() == ".bat"))
            tools.push_back(p.filename());
    }
}

void PluginManager::runTool(const string& name)
{
    string cmd = string(TOOLS_FOLDER) + "/\"" + name +"\"";
    system(cmd.c_str());
}

vector<string> PluginManager::get_tools() const
{
    return tools;
}
