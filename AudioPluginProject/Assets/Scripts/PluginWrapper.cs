using System.Collections;
using System.Collections.Generic;
using System.Runtime.InteropServices;
using UnityEngine;

public class PluginWrapper : MonoBehaviour
{
    #if UNITY_STANDALONE_WIN
        const string dll_name = "MyAudioPlugin";
    #elif UNITY_ANDROID
        const string dll_name = "MyAndroidAudioPlugin";
    #endif



    [DllImport(dll_name)]
    public static extern void InitAudioEngine();

    [DllImport(dll_name)]
    public static extern void UpdateAudioEngine();

    [DllImport(dll_name)]
    public static extern void ShutdownAudioEngine();

    [DllImport(dll_name)]
    public static extern int LoadSound(string path, bool loop);

    [DllImport(dll_name)]
    public static extern int PlaySounds(string path);

    [DllImport(dll_name)]
    public static extern int SetListener(Vector3 pos, Vector3 forward, Vector3 up);

    [DllImport(dll_name)]
    public static extern int SetSources(AudioSourceStruct[] array, int size);

    [DllImport(dll_name)]
    public static extern int SpatializeSourcesAndAudio();

    public static void UInitAudioEngine()
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
            InitAudioEngine();
        #else
            return;
        #endif
    }

    public static void UUpdateAudioEngine()
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
                UpdateAudioEngine();
        #else
                    return;
        #endif
    }

    public static void UShutdownAudioEngine()
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
        {
            ShutdownAudioEngine();
            return;
        }
        #else
                    return;
        #endif
    }

    public static int ULoadSound(string path, bool loop)
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
               return LoadSound(path, loop);
        #else
                    return -1;
        #endif
    }

    public static int UPlaySounds(string path)
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
                return PlaySounds(path);
        #else
                    return -1;
        #endif
    }

    public static int USetListener(Vector3 pos, Vector3 forward, Vector3 up)
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
               return SetListener(pos, forward, up);
        #else
                    return -1;
        #endif
    }

    public static int USetSources(AudioSourceStruct[] array, int size)
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
                return SetSources(array, size);
        #else
                            return -1;
        #endif

    }

    public static int USpatializeSourcesAndAudio()
    {
        #if (UNITY_ANDROID || UNITY_STANDALONE_WIN)
                return SpatializeSourcesAndAudio();
        #else
                                    return -1;
        #endif

    }
}
