#include "precompile.h"
#include "rboot.h"

#include "jcore.h"
#include "iwindowserver.h"
#include "jdialog.h"
#include "isoundserver.h"
#include "jdialog.h"
#include "modelviewer.h"
#include "path.h"
#include "imodelserver.h"
#include "jmodelanim.h"
#include "fstream.h"
#include "ifileserver.h"
#include "commandline.h"
#include "ipersistserver.h"


// FIXME:
#ifdef __GNUC__
void GetExecutablePath( char* path, int nMaxPath )
{
}
void ImportRenderLib()
{
    import( rb_rendergl );
}
const char* c_RenderServerName = "renderservergl";
#else
#include "windows.h"
void GetExecutablePath( char* path, int nMaxPath )
{
    GetModuleFileName( GetModuleHandle( NULL ), path, nMaxPath );
}
void ImportRenderLib()
{
    import( rb_render9 );
}
const char* c_RenderServerName = "renderserverdx9";
#endif

RBoot g_Boot;

RBoot::RBoot()
    : m_pCore( NULL )
{
}

bool RBoot::Init( const CommandLine& cmd )
{
    import( rb_core         );

    JCore* m_pCore = new JCore();
    m_pCore->SetName( "core" );

    if (m_pCore != JCore::s_pInstance)
    {
        return false;
    }

    uint32_t hwnd = 0;
    const char* pHandle = cmd.GetValue( "window" );
    if (pHandle)
    {
        sscanf( pHandle, "%d", &hwnd );
    }

    import( rb_draw         );
    //import( rb_extui        );
    import( rb_particle     );
    ImportRenderLib();
    import( rb_scene        );
    import( rb_script_lua   );
    //import( rb_sound        );
    //import( rb_texture      );
    import( rb_ui           );
    //import( rb_video        );
    //import( rb_physics      );

    link_class( ModelViewer );
    link_class( TestDriver  );

    m_pCore->Init();

    AddCommonMediaPath();

    const char* scriptFile = cmd.GetValue( "script" );
    JObject* pRoot = NULL;
    if (scriptFile)
    {
        //  find script's media directory and add it
        AddScriptMediaPath( scriptFile );
        //  load script
        pRoot = g_pPersistServer->LoadObject( scriptFile );
    }
    else
    {
        AddModuleMediaPath();
    }

    const char* mediaDir = cmd.GetValue( "media" );
    g_pFileServer->AddMediaPath( mediaDir );

    m_pCore->AddServer( "animserver"      );
    m_pCore->AddServer( "stringserver"    );
    m_pCore->AddServer( "windowserver"    );
    m_pCore->AddServer( c_RenderServerName );
    m_pCore->AddServer( "drawserver"      );
    m_pCore->AddServer( "modelserver"     );
    m_pCore->AddServer( "particleserver"  );
    m_pCore->AddServer( "luaserver"       );
    m_pCore->AddServer( "soundserver"     );
    m_pCore->AddServer( "physicsserver"   );
    m_pCore->AddServer( "videoserver"     );

    g_pWindowServer->SetRootHandle( reinterpret_cast<void*>( hwnd ) );

    m_pCore->InitTree();

    if (!pRoot)
    {
        pRoot = g_pPersistServer->LoadObject( "rboot" );
    }
    if (!pRoot)
    {
        return false;
    }

    JString objPath;
    pRoot->GetPath( objPath );
    m_pCore->SetRootObject( objPath.c_str() );

    g_pWindowServer->AddWindow( pRoot );
    pRoot->InitTree();

    ModelViewer* pModelViewer = obj_cast<ModelViewer>( pRoot );
    Path animPath, modelPath;
    if (pModelViewer)
    {
        pModelViewer->SetAnim( cmd.GetValue( "anim" ) );
        pModelViewer->SetModel( cmd.GetValue( "model" ) );
    }

    JDialog* pRootDlg = obj_cast<JDialog>( pRoot );
    if (pRootDlg)
    {
        pRootDlg->Show();
    }

    return true;
}

int RBoot::Run()
{
    return g_pWindowServer->RunApplicationLoop();
}

void RBoot::Shut()
{
    if (m_pCore)
    {
        m_pCore->Release();
    }
}

void RBoot::Update()
{
    g_pWindowServer->RenderFrame();
}

void RBoot::AddModuleMediaPath()
{
    //  set current working directory to the same where we are located
    char path[_MAX_PATH];
    GetExecutablePath( path, _MAX_PATH );
    Path mediaPath( path );
    mediaPath.SetFileExt( "" );
    mediaPath.DirAppend( "media" );
    if (mediaPath.Exists())
    {
        g_pFileServer->AddMediaPath( mediaPath.GetFullPath() );
    }
}

void RBoot::AddCommonMediaPath()
{
    char path[_MAX_PATH];
    GetExecutablePath( path, _MAX_PATH );
    Path mediaPath( path );
    mediaPath.SetFileExt( "" );
    mediaPath.DirUp();
    mediaPath.DirAppend( "media" );
    if (mediaPath.Exists())
    {
        g_pFileServer->AddMediaPath( mediaPath.GetFullPath() );
    }
}

void RBoot::AddScriptMediaPath( const char* scriptFile )
{
    Path path( scriptFile );
    const char* cdir = path.GetDriveDir();
    const char* pMedia = strstr( cdir, "/media/" );
    if (pMedia == NULL)
    {
        pMedia = strstr( cdir, "\\media\\" );
    }
    if (pMedia)
    {
        JString mdir( cdir, pMedia - cdir + strlen( "media" ) + 1 );
        mdir += "\\";
        g_pFileServer->AddMediaPath( mdir.c_str() );
    }
}
