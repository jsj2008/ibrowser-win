/**********************************************************\

  Auto-generated ibrowserAPI.h

\**********************************************************/

#include <string>
#include <sstream>
#include <boost/weak_ptr.hpp>
#include "JSAPIAuto.h"
#include "BrowserHost.h"
#include "ibrowser.h"

extern "C"{
#include <libimobiledevice/libimobiledevice.h>
#include <libimobiledevice/lockdown.h>
#include <libimobiledevice/installation_proxy.h>
#include <libimobiledevice/afc.h>
#include <libimobiledevice/sbservices.h>
#include <libimobiledevice/service.h>
}

#ifndef H_ibrowserAPI
#define H_ibrowserAPI

#define log(fmt, ...)   printf(fmt,## __VA_ARGS__);printf("\n")

#define F_SUCC  const boost::optional<FB::JSObjectPtr>& scb
#define F_ERRO  const boost::optional<FB::JSObjectPtr>& ecb
#define F_T_FLAG    boost::optional<bool> noThread
#define F_ADD F_SUCC,F_ERRO,F_T_FLAG
#define SUCC(a1,...)    \
    do{ \
        if(scb && (*scb)->isValid()){ \
            (*scb)->InvokeAsync("", FB::variant_list_of( a1,## __VA_ARGS__ )); \
        } \
    }while(0)
#define ERRO(msg)   \
    do{ \
        if(ecb && (*ecb)->isValid()){ \
            (*ecb)->InvokeAsync("", FB::variant_list_of(msg)); \
        } \
        printf("error:%s\n",msg); \
        clean(); \
        return false; \
    }while(0)
#define THREAD(fun,...) \
    do{ \
        if(!noThread && scb && (*scb)->isValid() ) \
        { \
            printf("use thread:%s\n",__FUNCTION__); \
            boost::thread t(boost::bind(fun,this,## __VA_ARGS__,scb,ecb,true)); \
            return true; \
        } \
        if(!init(scb,ecb)) return false; \
    }while(0)

class ibrowserAPI;

class Callback{
public:
    void set(const char* fn,FB::JSObjectPtr op)
    {
        list[fn]=op;
    }
    FB::JSObjectPtr get(const char* fn)
    {
        return list[fn];
    }
    
private:
    std::map<const char*,FB::JSObjectPtr> list;
};

class DownloadConfig {
public:
    DownloadConfig(int id,
                   std::string url,
                   FILE *stream,
                   FB::JSObjectPtr pcb,
                   double filesize,
                   double start,
                   double end,
                   std::vector<double> *counter)
    {
        this->id=id;
        this->url=url;
        this->stream=stream;
        this->pcb=pcb;
        this->filesize=filesize;
        this->start=start;
        this->end=end;
        this->counter=counter;
    };
    int id;
    std::string url;
    FILE *stream;
    FB::JSObjectPtr pcb;
    double filesize;
    double start;
    double end;
    std::vector<double> *counter;
    
    double getDownloadedSize()
    {
        double done=0;
        int len=this->counter->size();
        for(int i=0;i<len;i++)
        {
            done+=this->counter->at(i);
        }
        return done;
    }
    
};

class ibrowserAPI : public FB::JSAPIAuto
{
public:
    ////////////////////////////////////////////////////////////////////////////
    /// @fn ibrowserAPI::ibrowserAPI(const ibrowserPtr& plugin, const FB::BrowserHostPtr host)
    ///
    /// @brief  Constructor for your JSAPI object.
    ///         You should register your methods, properties, and events
    ///         that should be accessible to Javascript from here.
    ///
    /// @see FB::JSAPIAuto::registerMethod
    /// @see FB::JSAPIAuto::registerProperty
    /// @see FB::JSAPIAuto::registerEvent
    ////////////////////////////////////////////////////////////////////////////
    ibrowserAPI(const ibrowserPtr& plugin, const FB::BrowserHostPtr& host) :
        m_plugin(plugin), m_host(host) , uploadFileDir("/Downloads"), downloadThreads(10)
    {
		registerMethod("setIdeviceEventCallback", make_method(this, &ibrowserAPI::setIdeviceEventCallback));
        registerMethod("echo",      make_method(this, &ibrowserAPI::echo));
        /*
        registerMethod("getDeviceInfo", make_method(this, &ibrowserAPI::getDeviceInfo));
        registerMethod("getAppList",      make_method(this, &ibrowserAPI::getAppList));
        registerMethod("getSbservicesIconPngdata", make_method(this, &ibrowserAPI::getSbservicesIconPngdata));
        registerMethod("openDialog", make_method(this, &ibrowserAPI::openDialog));
        registerMethod("uploadFile", make_method(this, &ibrowserAPI::uploadFile));
        registerMethod("installPackage", make_method(this, &ibrowserAPI::installPackage));
        registerMethod("uninstallPackage", make_method(this, &ibrowserAPI::uninstallPackage));
        registerMethod("downloadFile", make_method(this, &ibrowserAPI::downloadFile));*/
        
        // Read-only property
        registerProperty("version",
                         make_property(this,
                                       &ibrowserAPI::get_version));
        
        printf("new ibrowserAPI\n");
        
        
    }

    ///////////////////////////////////////////////////////////////////////////////
    /// @fn ibrowserAPI::~ibrowserAPI()
    ///
    /// @brief  Destructor.  Remember that this object will not be released until
    ///         the browser is done with it; this will almost definitely be after
    ///         the plugin is released.
    ///////////////////////////////////////////////////////////////////////////////
    virtual ~ibrowserAPI() {};

    ibrowserPtr getPlugin();

    // Read-only property ${PROPERTY.ident}
    std::string get_version();

    // Method echo
    FB::variant echo(const FB::variant& msg);
    
    /*
    FB::variant getDeviceInfo(const std::vector<std::string>& domain,F_ADD);
    FB::variant getAppList(F_ADD);
    FB::variant getSbservicesIconPngdata(const std::string& bundleId,F_ADD);
    FB::variant openDialog(F_ADD);
    FB::variant uploadFile(const std::string& fileName,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    FB::variant installPackage(const std::string& fileName,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    FB::variant uninstallPackage(const std::string& fileName,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    FB::variant downloadFile(const std::string& url,const std::string& filename,const boost::optional<FB::JSObjectPtr>& pcb, F_ADD);
    static void* downloadThread(void *data);
    static int downloadWrite(void *buffer, size_t size, size_t nmemb, void *stream);
    static int downloadProgress(void* ptr, double rDlTotal, double rDlNow, double rUlTotal, double rUlNow);
    static void installCallback(const char *operation, plist_t status, void *user_data);*/
    FB::variant setIdeviceEventCallback(const FB::JSObjectPtr& callback);
    static void ideviceEventCallback(const idevice_event_t *event, void *user_data);
    
    
    // Event helpers
    FB_JSAPI_EVENT(test, 0, ());
    FB_JSAPI_EVENT(echo, 2, (const FB::variant&, const int));

private:
    ibrowserWeakPtr m_plugin;
    FB::BrowserHostPtr m_host;

    std::string uploadFileDir;
    unsigned int downloadThreads;
    
    idevice_t device;
    instproxy_client_t instproxy_client;
    lockdownd_client_t 	lockdownd_client;
    sbservices_client_t sbservices_client;
    afc_client_t afc_client;
    
    
    bool init(F_SUCC,F_ERRO);
    void clean();
    
};

#endif // H_ibrowserAPI

