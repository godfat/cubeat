
#include "utils/Logger.hpp"

#if defined(_SHOOTING_CUBES_ANDROID_)

#include "android/unpack_data.h"
#include "android/unzip.h" // yes, for unzipping
#include <android_native_app_glue.h>
#include <string>  // for path string manipulation at init
#include <errno.h>  // for ENOENT
#include <sys/stat.h> // for "stat"

char const* audiodirs[] = {
  "1",
  "1/a",
  "1/b",
  "1/c",
  "1/d",
  "1/e",
  "1/g",
  "2",
  "3",
  "3/3c",
  "3/3c",
  "3/3d",
  "4",
  "char1",
  "char2",
  "char3",
  "char3_new",
  "char4",
  "char5",
  "jungle",
  "jungle1",
  "jungle2",
  "jungle3",
  "jungle4",
  "jungle5",
  "jungle6",
  0
};

char const* audiofiles[] = {
  "count.wav",
  "day_a.ogg",
  "day_b.ogg",
  "ending.ogg",
  "go.wav",
  "smb_warning.wav",
  "title_a.ogg",
  "title_b.ogg",
  "1/a/1a-1.wav",
  "1/a/1a-2.wav",
  "1/a/1a-3.wav",
  "1/a/1a-4.wav",
  "1/b/HitB@11.wav",
  "1/c/GlassF@11.wav",
  "1/d/ShotA@11.wav",
  "1/e/getitem.wav",
  "1/g/09.wav",
  "2/2b_1.wav",
  "2/2b_2.wav",
  "2/2b_3.wav",
  "2/2b_4.wav",
  "2/2b_5.wav",
  "2/2b_6.wav",
  "2/2b_7.wav",
  "3/3c/win.wav",
  "3/3c/lose.wav",
  "3/3d/alarm.wav",
  "4/4a.wav",
  "4/4a2.wav",
  "4/4b.wav",
  "4/4c.wav",
  "char1/attack.wav",
  "char1/hit.wav",
  "char2/attack.wav",
  "char2/hit.wav",
  "char3/attack.wav",
  "char3/hit.wav",
  "char3_new/attack.wav",
  "char3_new/hit.wav",
  "char4/attack.wav",
  "char4/hit.wav",
  "char5/attack.wav",
  "char5/hit.wav",
  "jungle/bgm_a.ogg",
  "jungle/bgm_b.ogg",
  "jungle/bgm_quick_a.ogg",
  "jungle/bgm_quick_b.ogg",
  "jungle1/bgm_a.ogg",
  "jungle1/bgm_b.ogg",
  "jungle1/bgm_quick_a.ogg",
  "jungle1/bgm_quick_b.ogg",
  "jungle2/bgm_a.ogg",
  "jungle2/bgm_b.ogg",
  "jungle2/bgm_quick_a.ogg",
  "jungle2/bgm_quick_b.ogg",
  "jungle3/bgm_a.ogg",
  "jungle3/bgm_b.ogg",
  "jungle3/bgm_quick_a.ogg",
  "jungle3/bgm_quick_b.ogg",
  "jungle4/bgm_a.ogg",
  "jungle4/bgm_b.ogg",
  "jungle4/bgm_quick_a.ogg",
  "jungle4/bgm_quick_b.ogg",
  "jungle5/bgm_a.ogg",
  "jungle5/bgm_b.ogg",
  "jungle5/bgm_quick_a.ogg",
  "jungle5/bgm_quick_b.ogg",
  "jungle6/bgm_a.ogg",
  "jungle6/bgm_b.ogg",
  "jungle6/bgm_quick_a.ogg",
  "jungle6/bgm_quick_b.ogg",
  0
};

static void extract_from_apk_to_path(AAssetManager* mgr, char const* from, char const* to, char const* mode)
{
    AAsset* file_asset = AAssetManager_open(mgr, from, AASSET_MODE_BUFFER);
    const void* data = AAsset_getBuffer(file_asset);
    const off_t len = AAsset_getLength(file_asset);
    FILE* file = fopen(to, mode);
    if (NULL == file)
    {
        char buf[256]={0};
        sprintf(buf, "Unable to create (%s) file at: %s\n", mode, to);
        LOGE(buf);
    }
    else
    {
        char buf[256]={0};
        sprintf(buf, "asset: %s to path: (%s) %s\n", from, mode, to);
        LOGI(buf);
        int res = -1;
        res = fwrite(data, sizeof(char), len, file);
        if (len != res)
        {
            LOGE("Error writing file.\n");
        }
    }
    fclose(file);
    AAsset_close(file_asset);
}

void unpack_data(android_app* state)
{
    ANativeActivity* nativeActivity = state->activity;
    std::string dataPath(nativeActivity->internalDataPath); // internalDataPath points directly to the files/ directory
    std::string exPath(nativeActivity->externalDataPath);
    dataPath += "/"; // add trailling backslash....
    exPath += "/";
    std::string configFile = dataPath + "rc/config.zzml";
    std::string temp_zip_loc = dataPath + "config_script.zip";

    // sometimes if this is the first time we run the app
    // then we need to create the internal storage "files" directory
    
    struct stat sb;
    int32_t res = stat(dataPath.c_str(), &sb);
    if (0 == res && sb.st_mode & S_IFDIR)
    {
        LOGI("'files/' dir already in app's internal data storage.");
    }
    else if (ENOENT == res)
    {
        res = mkdir(dataPath.c_str(), 0770);
    }

    if (0 == res)
    {
        // test to see if the config file is already present
        res = stat(configFile.c_str(), &sb);
        if (0 == res && sb.st_mode & S_IFREG)
        {
            LOGI("Application config file already present");
            return;
        }
        else
        {
            LOGI("Application config file does not exist. Creating it ...");
            // read our application config file from the assets inside the apk
            // save the config file contents in the application's internal storage
            LOGI("Reading config zip package using the asset manager.\n");

            AAssetManager* assetManager = nativeActivity->assetManager;
            extract_from_apk_to_path(nativeActivity->assetManager, "config_script.zip", temp_zip_loc.c_str(), "w+");
            
            LOGI("Copying audio files out of assets....\n");
            
            mkdir( exPath.substr(0, exPath.length()-6 ).c_str(), 0770 ); // mkdir com.teampsc.cubeat (less /files) to sdcard
            mkdir( exPath.c_str(), 0770 );                // mkdir /files to sdcard in com.teampsc.cubeat
            mkdir( (exPath + "rc").c_str(), 0770 );       // mkdir /files/rc to sdcard in com.teampsc.cubeat
            mkdir( (exPath + "rc/sound").c_str(), 0770 ); // mkdir /files/rc to sdcard in com.teampsc.cubeat
            // now we need make all dir manually here, fuck android ...
            int i = 0; 
            while( audiodirs[i] != 0 ) {
              std::string f = std::string("rc/sound/") + audiodirs[i];
              mkdir( (exPath + f).c_str(), 0770 );
              ++i;
            }
            
            i = 0;
            while( audiofiles[i] != 0 ) {
              std::string f = std::string("rc/sound/") + audiofiles[i];
              std::string t = ( exPath + "rc/sound/" ) + audiofiles[i];
              extract_from_apk_to_path(nativeActivity->assetManager, f.c_str(), t.c_str(), "w+");
              ++i;
            }
        }
    }

    //Open and Extract the ZIP archive in to files/

    const int MAX_FILENAME = 512;
    const int READ_SIZE    = 8192;
    unzFile   zipfile;
    unz_global_info global_info;    // Get info about the zip file
    char read_buffer[ READ_SIZE ];  // Buffer to hold data read from the zip file.

    if ( (zipfile = unzOpen( temp_zip_loc.c_str() )) == NULL ) {
        LOGE( "minizip %s: not found\n" );
        return;
    }

    if ( unzGetGlobalInfo( zipfile, &global_info ) != UNZ_OK ) {
        LOGE( "minizip could not read file global info\n" );
        unzClose( zipfile );
        return;
    }

    for ( size_t i = 0; i < global_info.number_entry; ++i )
    {
        unz_file_info file_info;
        char filename[ MAX_FILENAME ];
        if ( unzGetCurrentFileInfo(zipfile, &file_info, filename, MAX_FILENAME, NULL, 0, NULL, 0 ) != UNZ_OK ) {
            LOGE( "minizip could not read file info\n" );
            unzClose( zipfile );
            return;
        }

        // Check if this entry is a directory or file, if so, create it
        const size_t filename_length = strlen( filename );
        if ( filename[ filename_length-1 ] == '/' ) {
            LOGI( "minizip dir:%s\n", filename );
            mkdir( (dataPath + filename).c_str(), 0770 );
        }
        else
        {
            // Entry is a file, so extract it.
            LOGI( "minizip file:%s\n", filename );
            if ( unzOpenCurrentFile( zipfile ) != UNZ_OK ) {
                LOGE( "could not open file\n" );
                unzClose( zipfile );
                return;
            }

            // Open a file to write out the data.
            FILE *out = fopen( (dataPath + filename).c_str(), "wb" );
            if ( out == NULL ) {
                LOGE( "minizip could not open destination file\n" );
                unzCloseCurrentFile( zipfile );
                unzClose( zipfile );
                return;
            }

            int res = UNZ_OK;
            do {
                res = unzReadCurrentFile( zipfile, read_buffer, READ_SIZE );
                if ( res < 0 ) {
                    LOGE( "minizip error %d\n", res );
                    unzCloseCurrentFile( zipfile );
                    unzClose( zipfile );
                    return;
                }

                if ( res > 0 ) {
                    if( fwrite( read_buffer, res, 1, out ) != 1 ) { // we write 1 element only, so check with 1
                        LOGE( "minizip fwrite error\n", res );
                        unzCloseCurrentFile( zipfile );
                        unzClose( zipfile );
                        return;
                    }
                }
            } while ( res > 0 ); // res > 0 means the number of byte copied if somes bytes are copied

            fclose( out );
        }

        unzCloseCurrentFile( zipfile );

        // Go the the next entry listed in the zip file.
        if ( ( i+1 ) < global_info.number_entry ) {
            if ( unzGoToNextFile( zipfile ) != UNZ_OK ) {
                LOGE( "minizip could not read next file\n" );
                unzClose( zipfile );
                return;
            }
        }
    }
    unzClose( zipfile );
}
#endif //_SHOOTING_CUBES_ANDROID_
