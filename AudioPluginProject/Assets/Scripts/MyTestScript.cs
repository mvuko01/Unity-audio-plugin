using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using UnityEditor;
using UnityEngine;
 
[System.Serializable]
public class AudioSourceObjects 
{
    public GameObject sources;
    public float min;
    public float max;
}

[StructLayout(LayoutKind.Sequential)]
public struct AudioSourceStruct
{
   public Vector3 position;
    public float min;
    public float max;

}

public class MyTestScript : MonoBehaviour
{

    
    public GameObject listener;
    public AudioSourceObjects[] sourceObjects;
   



    void Start()
    {

        PluginWrapper.UInitAudioEngine();

       
        


        DirectoryInfo dir = new DirectoryInfo(Application.streamingAssetsPath);
        FileInfo[] allFile = dir.GetFiles("*.mp3"); //nece ucitat mp3 datoteke
        string path;

        foreach (FileInfo file in allFile)
        {
            
            path = Path.Combine(Application.streamingAssetsPath, file.Name);
           

            Debug.Log(PluginWrapper.ULoadSound(path, true));
            PluginWrapper.UPlaySounds(path);
        }

    
    }

    
    void Update()
    {
        Vector3 pos_of_listener = listener.transform.position;
        Vector3 nul = Vector3.zero;
        PluginWrapper.USetListener(pos_of_listener, listener.transform.forward, Vector3.up);

        AudioSourceStruct[] sources = new AudioSourceStruct[sourceObjects.Length];

        for (int i = 0; i < sourceObjects.Length; i++)
        {
            sources[i].position = sourceObjects[i].sources.transform.position;
            sources[i].min = sourceObjects[i].min;
            sources[i].max = sourceObjects[i].max;

        }

        PluginWrapper.USetSources(sources, sourceObjects.Length);
        
        PluginWrapper.USpatializeSourcesAndAudio();
        PluginWrapper.UUpdateAudioEngine();
        


    }

    private void OnApplicationQuit()
    {
        PluginWrapper.UShutdownAudioEngine();
    }


}
