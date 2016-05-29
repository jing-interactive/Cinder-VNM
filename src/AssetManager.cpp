#include "AssetManager.h"
#include "cinder/app/App.h"
#include "cinder/ImageIo.h"
#include "cinder/Function.h"
#include "cinder/Utilities.h"
#include "cinder/ObjLoader.h"
#include "cinder/Log.h"
#include <map>

using namespace std;
using namespace ci;
using namespace ci::app;

namespace
{
    template <typename T> 
    T& getAssetResource(const string& relativeName, function<T(const string&, const string&)> loadFunc, const string& relativeNameB = "")
    {
        typedef map<string, T> MapType;
        static MapType sMap;
        static T nullResource;
        auto it = sMap.find(relativeName+relativeNameB);
        if (it != sMap.end())
        {
            return it->second;
        }
        CI_LOG_V( "Loading: " << relativeName << " " << relativeNameB);

        try
        {
            auto aPath = getAssetPath("") / relativeName;
            auto bPath = getAssetPath("") / relativeNameB;
            
            auto resource = loadFunc(aPath.string(), bPath.string());
            return sMap[relativeName+relativeNameB] = resource;
        }
        catch (Exception& e)
        {
            CI_LOG_EXCEPTION( "getAssetResource", e);
            return nullResource;
        }
    }
}

namespace am
{
    static SurfaceRef loadSurface(const string& absoluteName, const string&)
    {
        auto source = loadImage(absoluteName);
        return Surface::create(source);
    }

    SurfaceRef& surface(const string& relativeName)
    {
        return getAssetResource<SurfaceRef>(relativeName, loadSurface);
    }

    static gl::TextureRef loadTexture(const string& absoluteName, const string&)
    {
        auto source = loadImage(absoluteName);
        auto format = gl::Texture2d::Format().loadTopDown();

        return gl::Texture::create(source, format);
    }

    gl::TextureRef& texture(const string& relativeName)
    {
        return getAssetResource<gl::TextureRef>(relativeName, loadTexture);
    }

    static TriMeshRef loadTriMesh(const string& absoluteName, const string&)
    {
        auto source = DataSourcePath::create(absoluteName);
        auto ext = fs::path(absoluteName).extension();
        TriMeshRef mesh;
        
        if (ext == ".obj")
        {
            ObjLoader loader( source );
            mesh = TriMesh::create(loader);
        }
        else if (ext == ".msh")
        {
            mesh = TriMesh::create();
            mesh->read(source);
        }
        else
        {
            CI_LOG_W( "Unsupported mesh format: " << absoluteName );
            return nullptr;
        }
        
        if( !mesh->hasNormals() ) {
            mesh->recalculateNormals();
        }
        
        if( ! mesh->hasTangents() ) {
            mesh->recalculateTangents();
        }
        
        return mesh;
    }

    TriMeshRef& triMesh(const string& relativeName)
    {
        return getAssetResource<TriMeshRef>(relativeName, loadTriMesh);
    }

    static gl::VboMeshRef loadVboMesh(const string& absoluteName, const string&)
    {
        gl::VboMesh::Layout layout;
        auto triMesh = loadTriMesh(absoluteName, "");
        return gl::VboMesh::create(*triMesh);
    }

    gl::VboMeshRef& vboMesh(const string& relativeName)
    {
        return getAssetResource<gl::VboMeshRef>(relativeName, loadVboMesh);
    }

    static gl::GlslProgRef loadGlslProg(const string& vsAbsoluteName, const string& fsAbsoluteName)
    {
        gl::GlslProg::Format format;
#if defined( CINDER_GL_ES )
        format.version(300); // es 3.0
#else
        format.version(150); // gl 3.2
#endif
        format.vertex(DataSourcePath::create(vsAbsoluteName));
        format.fragment(DataSourcePath::create(fsAbsoluteName));
                
        return gl::GlslProg::create(format);
    }

    gl::GlslProgRef& glslProg(const string& vsFileName, const string& fsFileName)
    {
        return getAssetResource<gl::GlslProgRef>(vsFileName, loadGlslProg, fsFileName);
    }

    static string loadStr(const string& absoluteName, const string&)
    {
        return loadString(DataSourcePath::create(absoluteName));
    }

    string& str(const string& relativeName)
    {
        return getAssetResource<string>(relativeName, loadStr);
    }

    static vector<string> loadFiles(const string& absoluteFolderName, const string&)
    {
        vector<string> files;
        fs::directory_iterator kEnd;
        for (fs::directory_iterator it(absoluteFolderName); it != kEnd; ++it)
        {
            if (fs::is_regular_file(*it) && it->path().extension() != ".db" 
                && it->path().extension() != ".DS_Store")
            {
#ifdef _DEBUG
                //console() << it->path() << endl;
#endif
                files.push_back(it->path().generic_string());
            }
        }
        return files;
    }

    vector<string> files(const string& relativeFolderName)
    {
        return getAssetResource<vector<string>>(relativeFolderName, loadFiles);
    }
}
