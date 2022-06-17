using System;
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
    private AudioSourceStruct[] sources;




  

    void Start()
    {

        PluginWrapper.UInitAudioEngine();
        sources = new AudioSourceStruct[sourceObjects.Length];


        string audioName = "0001724.mp3";
        string _path = System.IO.Path.Combine(Application.streamingAssetsPath, audioName);
        UnityEngine.Networking.UnityWebRequest www = UnityEngine.Networking.UnityWebRequest.Get(_path);
        
        www.SendWebRequest();
        while (!www.isDone)
        {
        }
    

        string filepath = Path.Combine(Application.persistentDataPath, audioName);
        File.WriteAllBytes(filepath, www.downloadHandler.data);


        PluginWrapper.ULoadSound(filepath, true);
        PluginWrapper.UPlaySounds(filepath);

    }


    void Update()
    {
         
        PluginWrapper.USetListener(listener.transform.position, listener.transform.forward, Vector3.up);

        

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
