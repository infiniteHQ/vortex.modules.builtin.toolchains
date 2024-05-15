
#ifndef DISABLE_MODULE

/**
 * Warning : Recursive dependance for vortex.modules.builtin.tasklists
 * 
 * @brief This toolchain module is used in tasklists module only as header library, the tasklists module don't interact with this module's stuffs
 * 
 * */
#include <Vortex/modules/vortex.modules.builtin.tasklists.1.1.0/src/module.h>
/**/

#include <Vortex/modules/vortex.modules.builtin.tasks.1.1.0/src/module.h>
#include <Vortex/modules/vortex.modules.builtin.packages.1.1.0/src/module.h>
#include <Vortex/modules/vortex.modules.builtin.packages.1.1.0/src/instances/packageInstance/PackageRenderInstance.h>

// FUTURE : #include <Vortex/1.1/main/include/vortex.h>
// When we run "vortex" this will run /usr/local/bin/Vortex/1.1/vortex
/// So we can scan selected version of a project and run it if the project version is installed

#include <Vortex/main/include/vortex.h>
#include <Vortex/main/include/vortex_internals.h>
#include <Vortex/tools/editor/app/include/editor.h>

#ifndef __TOOLCHAIN__MODULE_H__
#define __TOOLCHAIN__MODULE_H__

class Toolchain;
struct ToolchainsModuleCTX
{
  std::vector<std::shared_ptr<Toolchain>> m_toolchains;    
  std::shared_ptr<ModuleInterface>        m_interface;
};

#ifndef TOOLCHAIN_MODULE_API
#define TOOLCHAIN_MODULE_API
#endif

#ifndef CToolchainModule
extern TOOLCHAIN_MODULE_API ToolchainsModuleCTX *CToolchainModule; // Current implicit context pointer
#endif

namespace ToolchainModule{
    //TOOLCHAIN_MODULE_API void InitTasks();
    TOOLCHAIN_MODULE_API bool RegisterNewToolchain(std::shared_ptr<Toolchain> toolchain, nlohmann::json toolchainData);


    // Toolchain Current System "TCS" functions.
    TOOLCHAIN_MODULE_API void CreateTCSTask(std::string tasktype, std::string component, std::string uniqueID, int priority, std::shared_ptr<hArgs> props);
    TOOLCHAIN_MODULE_API void PushTCSPackageReport(PackageReport report);
    TOOLCHAIN_MODULE_API void PushTCSSize(std::string newsize);
    TOOLCHAIN_MODULE_API void PopulateTCS(nlohmann::json jsonData); // from working_host.config
    TOOLCHAIN_MODULE_API nlohmann::json ExtractTCS();
    TOOLCHAIN_MODULE_API void SaveTCS(std::shared_ptr<Toolchain> toolchain);
    TOOLCHAIN_MODULE_API void PutTCSVariable(Task* task, std::string name, std::string createdBy, std::string value);
    TOOLCHAIN_MODULE_API std::tuple<std::string, std::string, std::string> GetTCSVariable(Task* task, std::string name);

    TOOLCHAIN_MODULE_API std::pair<std::string, int> exec_cmd(const std::string &cmd);
    TOOLCHAIN_MODULE_API std::pair<std::string, int> exec_cmd_quote(const std::string &cmd);

    // Toolchain main functions.
    TOOLCHAIN_MODULE_API void PushSave(std::shared_ptr<ToolchainSave> save);
    TOOLCHAIN_MODULE_API void PushDistSave(std::shared_ptr<VxDistToolchainSave> save);
    TOOLCHAIN_MODULE_API void CreateTasklist(std::string name, std::shared_ptr<ToolchainSave> save);
    TOOLCHAIN_MODULE_API void DeleteTasklist(std::string name);
    TOOLCHAIN_MODULE_API void RegisterTasklist(const std::string label);
    TOOLCHAIN_MODULE_API void PushDistSave();
    TOOLCHAIN_MODULE_API void FindTasklists();
    TOOLCHAIN_MODULE_API void Refresh();
    TOOLCHAIN_MODULE_API void InitTasks(const std::shared_ptr<hArgs>& args);
    TOOLCHAIN_MODULE_API void RefreshDistConfig();
    TOOLCHAIN_MODULE_API void RefreshCurrentWorkingToolchain();
    TOOLCHAIN_MODULE_API std::string GetTriplet(std::string triplet_type);
    TOOLCHAIN_MODULE_API bool TaskSuccedded(std::string taskID);
    TOOLCHAIN_MODULE_API void MakeSnapshot(std::string label);
    TOOLCHAIN_MODULE_API void MakeDistSnapshot(std::string label);
    TOOLCHAIN_MODULE_API void RetakeSnapshot(std::string snapshot_label);
    TOOLCHAIN_MODULE_API void RefreshSnapshots();
    TOOLCHAIN_MODULE_API void CreateCurrentToolchainSystem();
    TOOLCHAIN_MODULE_API void DeleteCurrentToolchainSystem();
    TOOLCHAIN_MODULE_API void DeployNewCurrentSystem();
    TOOLCHAIN_MODULE_API void DeleteCurrentSystem();
    TOOLCHAIN_MODULE_API void Init();
    TOOLCHAIN_MODULE_API void PreBuild();
    TOOLCHAIN_MODULE_API void Build();
    TOOLCHAIN_MODULE_API void PostBuild();
    TOOLCHAIN_MODULE_API void RegisterPackage(const std::string label, const std::string emplacemement);
    TOOLCHAIN_MODULE_API void FindPackages();
    TOOLCHAIN_MODULE_API void PreparePackage(std::string packageName);
    TOOLCHAIN_MODULE_API void CreateToolchainDirectory(/*VxDirectory*/);
}



struct ToolchainCurrentSystem{
    std::string lastUpdate;
    std::string size;

    std::vector<std::shared_ptr<Task>> executedTasks;
    std::shared_ptr<Toolchain> parent;

    std::vector<PackageReport> packageReports;
    std::vector<VxActionReport> actionReports;
    void CreateTask(std::string tasktype, std::string component, std::string uniqueID, int priority, std::shared_ptr<hArgs> props);

    void PushPackageReport(PackageReport report){this->packageReports.push_back(report);};
    void PushSize(std::string newsize){this->size = newsize;};
    void Populate(nlohmann::json jsonData); // from working_host.config
    nlohmann::json Extract();
    void Save(std::shared_ptr<Toolchain> toolchain);


    // Variable Name // Required By (Task) // Value
    std::vector<std::tuple<std::string, std::string, std::string>> variables;
    void put_varable(Task* task, std::string name, std::string createdBy, std::string value){
        task->created_variables.push_back(std::make_tuple(name, createdBy, value));
        this->variables.push_back(std::make_tuple(name, createdBy, value));
    }

    // Save dans le current system les variables utilisées
    std::tuple<std::string, std::string, std::string> get_varable(Task* task, std::string name){
        for(auto var : this->variables){
            if(std::get<0>(var) == name){
                task->used_variables.push_back(var);
                return var;
            }
        }
        task->used_variables.push_back({name, "unknow", "unknow"});
        return {name, "unknow", "unknow"};
    }   

};


struct ToolchainSnapshot{
    std::string date;
    std::string name;
    std::string path;

    ToolchainCurrentSystem snapshotSystem; // to import from 
};


struct Toolchain
{
    // Vortex project informations
    std::string name = "unknow";
    std::string author = "unknow";
    std::string description = "unknow";
    std::string version;
    std::string type = "unknow";
    std::string state = "unknow";
    std::string vendor = "unknow";
    std::string platform = "unknow";
    std::string configFilePath = "unknow";
    std::string path = "unknow";

    std::string builder_arch = "unknow";
    std::string builder_vendor = "unknow";
    std::string builder_platform = "unknow";
    std::string builder_cpu = "unknow";
    std::string builder_fpu = "unknow";

    std::string target_arch = "unknow";
    std::string target_vendor = "unknow";
    std::string target_platform = "unknow";
    std::string target_cpu = "unknow";
    std::string target_fpu = "unknow";

    std::string host_arch = "unknow";
    std::string host_vendor = "unknow";
    std::string host_platform = "unknow";
    std::string host_cpu = "unknow";
    std::string host_fpu = "unknow";

    std::string toolchain_type = "unknow";

    std::string packages_data = "unknow";
    std::string envPath = "unknow";

    std::string path_hostsnapshots = "unknow";

    const char* pool_name = "unknow";


    // Low level toolchain informations
    std::string workingPath = "not specified";
    std::string distPath = "not specified";
    std::string ouputPath = "not specified";
    std::string toolchainSourcePath = "not specified";
    bool useWorkingPath = false;
    bool isCompressed = false;
    std::string compressionMode = "not specified";

    bool haveCurrentSys;

    // PERMETTRE D'UTILISER LES ${} avec les variables ci dessous !

    std::string localPackagesPath;
    std::string localScriptsPath;
    std::string localPatchsPath;

    std::vector<std::shared_ptr<Task>> allTasks;

    // -> Toolchain Sysroot Path
    std::string sysrootPath = "not specified";
    std::string debugrootPath = "not specified";
    std::string vxToolchain_TempPackagesPoolPath = "not specified";
    std::string crosstoolsPath = "not specified";
    std::string distPackagesPath = "not specified";
    std::string currentBuildArch = "unknow";
    std::string vxToolchain_HostPath = "not specified";
    std::string vxToolchain_ProdPath = "not specified";

    // -> Source pool
    std::string vxToolchain_SourcesPool = "not specified";

    // -> Temporary source directory in toolchain
    std::string vxToolchain_SourcesToolchain = "not specified";

    // -> Complete triplet to handle or complete if specified
    std::string hostTriplet = "not specified";
    std::string builderTriplet = "not specified";
    std::string targetTriplet = "not specified";

    // -> Simple ARCH index
    std::string vxToolchain_BuilderArch = "not specified";
    std::string vxToolchain_TargetArch = "not specified";
    std::string vxToolchain_HostArch = "not specified";

    // Vector de packages

    std::vector<std::shared_ptr<PackageInterface>> registeredPackages;
    std::vector<std::shared_ptr<TasklistInterface>> registeredTasklists;

    std::vector<std::shared_ptr<Package>> packages;
    std::vector<std::shared_ptr<TaskList>> tasklists;
    // Scripts
    // Modules & other assets..
    // Patchs

    ToolchainCurrentSystem currentLoadedSystem;

    std::vector<std::shared_ptr<Task>> tasks;
    std::vector<ToolchainSnapshot> snapshots;

    std::shared_ptr<TaskProcessor> taskProcessor;

    std::pair<std::string, int> exec_cmd(const std::string &cmd);
    std::pair<std::string, int> exec_cmd_quote(const std::string &cmd);

    void PushSave(std::shared_ptr<ToolchainSave> save);
    void PushDistSave(std::shared_ptr<VxDistToolchainSave> save);

    VxDistToolchain distToolchain;

    void CreateTasklist(std::string name, std::shared_ptr<ToolchainSave> save);
    void DeleteTasklist(std::string name);

    void RegisterTasklist(const std::string label);

    void PushDistSave();

    // TO CLEAN
    void FindTasklists();
    void Refresh();
    void InitTasks();
    void RefreshDistConfig();
    void RefreshCurrentWorkingToolchain();
    std::string GetTriplet(std::string triplet_type);
    bool TaskSuccedded(std::string taskID);
    void MakeSnapshot(std::string label);
    void MakeDistSnapshot(std::string label);
    void RetakeSnapshot(std::string snapshot_label);
    void RefreshSnapshots();
    void CreateCurrentToolchainSystem();
    void DeleteCurrentToolchainSystem();
    void DeployNewCurrentSystem();
    void DeleteCurrentSystem();
    void Init();
    void PreBuild();
    void Build();
    void PostBuild();
    void RegisterPackage(const std::string label, const std::string emplacemement);
    void FindPackages();
    void PreparePackage(std::string packageName);
    //void ConfigurePackage(std::string packageName);
    //void CompilePackage(std::string packageName);
    //void InstallPackage(std::string packageName);

    void CreateToolchainDirectory(/*VxDirectory*/);


template<typename T>
void AddTaskType(const std::string& name);
};






static void RegisterToolchains()
{
  VxContext &ctx = *CVortexMaker;

  // clear existing dist toolchains
  CToolchainModule->m_toolchains.clear();

  // Toolchains
  for (const auto &file : VortexMaker::SearchFiles(CToolchainModule->m_interface->m_datapath, "toolchain.config"))
  {
    try
    {
      nlohmann::json filecontent = VortexMaker::DumpJSON(file);
      std::shared_ptr<Toolchain> toolchain = std::make_shared<Toolchain>();

      toolchain->configFilePath = file;
      toolchain->path = file;

      size_t position = toolchain->path.find("/toolchain.config");
      if (position != std::string::npos)
      {
        toolchain->path.erase(position, 17);
      }

      bool alreadyExist = false;

      for (auto alreadyRegistered : CToolchainModule->m_toolchains)
      {
        if (alreadyRegistered->name == filecontent["toolchain"]["name"].get<std::string>())
        {
          VortexMaker::LogError("Core", alreadyRegistered->name + " is already registered.");
          alreadyExist = true;
        }
      }

      if (alreadyExist == true)
      {
        continue;
      }

      ToolchainModule::RegisterNewToolchain(toolchain, filecontent);
    }
    catch (const std::exception &e)
    {
      std::cerr << "Error : " << e.what() << std::endl;
    }
  }
}



#endif // __TOOLCHAIN__MODULE_H__
#endif