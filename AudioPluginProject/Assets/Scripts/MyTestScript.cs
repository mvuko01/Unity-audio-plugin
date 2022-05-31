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
   



    void Start()
    {

     // PluginWrapper.UInitAudioEngine();





        /*DirectoryInfo dir = new DirectoryInfo(Application.streamingAssetsPath);
         FileInfo[] allFile = dir.GetFiles("*.wav"); //nece ucitat mp3 datoteke
         string path;

         foreach (FileInfo file in allFile)
         {
            Debug.Log(file.Name);
             path = Path.Combine(Application.streamingAssetsPath, file.Name);
            Debug.Log(path);

             
         }*/


        string name1 = "aaa.wav";
        string name2 = "bbb.wav";

        string asssetpath = "jar:file://" + Application.dataPath + "!assets/";

        string _path1 = System.IO.Path.Combine(Application.streamingAssetsPath, name1);
        string _path2 = Path.Combine(Application.streamingAssetsPath, name2);
        Debug.Log(_path1);
       
        UnityEngine.Networking.UnityWebRequest www1 = UnityEngine.Networking.UnityWebRequest.Get(_path1);
        UnityEngine.Networking.UnityWebRequest www2 = UnityEngine.Networking.UnityWebRequest.Get(_path2);

       www1.SendWebRequest();
        while (!www1.isDone)
        {
        }
        var filepath = string.Format(Application.persistentDataPath, name1);
        Debug.Log(filepath);
        File.WriteAllBytes(filepath, www1.downloadHandler.data);
        www2.SendWebRequest();
        while (!www2.isDone)
        {
        }
        
        /*string path2String = www2.downloadHandler.text;
        Debug.Log(path1String);*/

      //  PluginWrapper.ULoadSound(filepath, true);
       // PluginWrapper.ULoadSound(_path2, true);
       // PluginWrapper.UPlaySounds(filepath);
       // PluginWrapper.UPlaySounds(_path2);


    }


    void Update()
    {
        Vector3 pos_of_listener = listener.transform.position;
        Vector3 nul = Vector3.zero;
      //PluginWrapper.USetListener(pos_of_listener, listener.transform.forward, Vector3.up);

        AudioSourceStruct[] sources = new AudioSourceStruct[sourceObjects.Length];

        for (int i = 0; i < sourceObjects.Length; i++)
        {
            sources[i].position = sourceObjects[i].sources.transform.position;
            sources[i].min = sourceObjects[i].min;
            sources[i].max = sourceObjects[i].max;

        }

       //PluginWrapper.USetSources(sources, sourceObjects.Length);
        
        //PluginWrapper.USpatializeSourcesAndAudio();
      //PluginWrapper.UUpdateAudioEngine();
        


    }

    private void OnApplicationQuit()
    {
       //PluginWrapper.UShutdownAudioEngine();
    }


}
