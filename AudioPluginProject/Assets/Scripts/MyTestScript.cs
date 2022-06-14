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


    private void Awake()
    {
           
    }

    void Start()
    {

        PluginWrapper.UInitAudioEngine();





      


        string name1 = "0001724.mp3";
       // string name2 = "bbb.wav";


        string _path1 = System.IO.Path.Combine(Application.streamingAssetsPath, name1);
       // string _path2 = Path.Combine(Application.streamingAssetsPath, name2);
       

        UnityEngine.Networking.UnityWebRequest www1 = UnityEngine.Networking.UnityWebRequest.Get(_path1);
       // UnityEngine.Networking.UnityWebRequest www2 = UnityEngine.Networking.UnityWebRequest.Get(_path2);

        www1.SendWebRequest();
        while (!www1.isDone)
        {
        }
      /* www2.SendWebRequest();
        while (!www2.isDone)
        {
       }*/

        string filepath1 = Path.Combine(Application.persistentDataPath, name1);
        Debug.Log("Filepath " + filepath1);
        File.WriteAllBytes(filepath1, www1.downloadHandler.data);

       /* string filepath2 = Path.Combine(Application.persistentDataPath, name2);
        File.WriteAllBytes(filepath2, www2.downloadHandler.data);*/





        

        int rez = PluginWrapper.ULoadSound(filepath1, true);
        Debug.Log("Result of load sound: " + rez);
        rez = PluginWrapper.UPlaySounds(filepath1);
        Debug.Log("Result of play sound: " + rez);

        /*rez = PluginWrapper.ULoadSound(filepath2, true);
        Debug.Log("Result of load sound: " + rez);
        rez = PluginWrapper.UPlaySounds(filepath2);
        Debug.Log("Result of play sound: " + rez);*/


      


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
