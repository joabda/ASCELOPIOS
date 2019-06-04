#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include <vector>
#include <string>
#include <experimental/filesystem>

#define TOOLS_FOLDER "tools"

using namespace std;

class PluginManager
{
private:
    vector<string> tools;

public:
    PluginManager();
    void runTool(const string& name);
    vector<string> get_tools() const;
};

#endif // PLUGINMANAGER_H
