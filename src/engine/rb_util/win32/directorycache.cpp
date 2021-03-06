/***********************************************************************************/
//  File:   DirectoryCache.cpp
//  Date:   15.09.2005
//  Author: Ruslan Shestopalyuk
/***********************************************************************************/
#include "precompile.h"
#include "path.h"
#include "windows.h"
#include "directorycache.h"

/***********************************************************************************/
/*  Directory implementation
/***********************************************************************************/
bool Directory::FindFile( const char* name, const char* ext, char* path )
{
    char fname[_MAX_PATH];
    _makepath( fname, NULL, NULL, name, ext );
    int nFiles = m_Files.size();
    for (int i = 0; i < nFiles; i++)
    {
        if (!stricmp( fname, m_Files[i].c_str() ))
        {
            _makepath( path, NULL, m_Path.c_str(), name, ext );
            return true;
        }
    }

    int nDir = m_Subdirs.size();
    for (int i = 0; i < nDir; i++)
    {
        if (m_Subdirs[i].FindFile( name, ext, path )) return true;
    }
    return false;
} // Directory::FindFile

void Directory::Update()
{
    m_Files.clear();
    m_Subdirs.clear();

    WIN32_FIND_DATA fData;

    Path searchPath( m_Path.c_str() );
    searchPath.SetFile( "*" );
    searchPath.SetExt ( "*" );

    HANDLE h = FindFirstFile( searchPath.GetFullPath(), &fData );
    uint32_t err = GetLastError();
    if (h == INVALID_HANDLE_VALUE) return;

    do
    {
        const char* fname = fData.cFileName;
        if (fname[0] != '.') 
        {
            uint32_t flags = fData.dwFileAttributes;
            if (flags & FILE_ATTRIBUTE_DIRECTORY)
            {
                char path[_MAX_PATH];
                _makepath( path, NULL, m_Path.c_str(), fname, NULL );
                m_Subdirs.push_back( Directory( fname, path ) );
                m_Subdirs.back().Update();
            }
            else if (!(flags & FILE_ATTRIBUTE_SYSTEM)) 
            {
                m_Files.push_back( fname );
            }
        }
    } while (FindNextFile( h, &fData ));

    FindClose( h );
}

/***********************************************************************************/
/*  DirectoryCache implementation
/***********************************************************************************/
DirectoryCache::DirectoryCache( const char* root )
{
    AddRoot( root );
}

void DirectoryCache::AddRoot( const char* path )
{
    for (int i = 0; i < m_Roots.size(); i++)
    {
        if (!stricmp( m_Roots[i].m_Path.c_str(), path ))
        {
            return;
        }
    }
    m_Roots.push_back( Directory( path, path ) );
    UpdateCache();
}

bool DirectoryCache::FindFile( const char* name, const char* ext, char* path )
{
    int nRoots = m_Roots.size();
    for (int i = 0; i < nRoots; i++)
    {
        if (m_Roots[i].FindFile( name, ext, path )) return true;
    }
    return false;
}

void DirectoryCache::UpdateCache()
{
    for (int i = 0; i < m_Roots.size(); i++)
    {
        m_Roots[i].Update();
    }
}


