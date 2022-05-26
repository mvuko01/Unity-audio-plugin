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

    [DllImport("MyAudioPlugin")]
    public static extern void InitAudioEngine();

    [DllImport("MyAudioPlugin")]
    public static extern void UpdateAudioEngine();

    [DllImport("MyAudioPlugin")]
    public static extern void ShutdownAudioEngine();

    [DllImport("MyAudioPlugin")]
    public static extern int LoadSound(string path, bool loop);

    [DllImport("MyAudioPlugin")]
    public static extern int PlaySounds(string path);

    [DllImport("MyAudioPlugin")]
    public static extern int ReturnNumOfSounds();

    [DllImport("MyAudioPlugin")]
    public static extern int ReturnNumOfChannels();

    [DllImport("MyAudioPlugin")]
    public static extern void SetListener(Vector3 pos, Vector3 forward, Vector3 up);

    [DllImport("MyAudioPlugin")]
    public static extern int SetSources(AudioSourceStruct[] array, int size);


   

    [DllImport("MyAudioPlugin")]
    public static extern int SpatializeSourcesAndAudio();

   



    //public AudioClip firstClip;
    //public AudioClip secondClip;
    public GameObject listener;
    public AudioSourceObjects[] sourceObjects;
   

    int chID;




    void Start()
    {

        InitAudioEngine();

        
        


        DirectoryInfo dir = new DirectoryInfo(Application.streamingAssetsPath);
        FileInfo[] allFile = dir.GetFiles("*.wav"); //nece ucitat mp3 datoteke
        string path;

        foreach (FileInfo file in allFile)
        {
            
            path = Path.Combine(Application.streamingAssetsPath, file.Name);
           
            LoadSound(path, true);
            PlaySounds(path);
        }

        
       
        //string firstPath = Path.Combine(Application.streamingAssetsPath, "singing.wav");
        // Debug.Log(" path " + firstPath);



        // int rez = LoadSound(firstPath, false);
        //chID = PlaySounds(firstPath);



        //Debug.Log("ChannelID of PlaySound first: " + chID);

        //rez = PlaySounds(secondPath, 0.0f);
        //Debug.Log("Rezult of PlaySound second: " + rez);
        //PlaySounds(secondPath, 0.0f);
       





        

       
        //Debug.Log("Angle value: " + AngleValue());




    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos_of_listener = listener.transform.position;
        Vector3 nul = Vector3.zero;
        SetListener(pos_of_listener, listener.transform.forward, Vector3.up);

        AudioSourceStruct[] sources = new AudioSourceStruct[sourceObjects.Length];

        for (int i = 0; i < sourceObjects.Length; i++)
        {
            sources[i].position = sourceObjects[i].sources.transform.position;
            sources[i].min = sourceObjects[i].min;
            sources[i].max = sourceObjects[i].max;

        }

        SetSources(sources, sourceObjects.Length);
        // ChangeVolumeByDistance(chID);
        //float panVal = ChangePanByOrientation(chID);
        SpatializeSourcesAndAudio();
        UpdateAudioEngine();
        //Debug.Log("Angle value: " + AngleValue());
        //Debug.Log("Pan value: " + panVal);
        //Debug.Log("Angle value: " + listener.transform.forward);


        //Debug.Log("Up value: " + Vector3.forward);


    }

    private void OnApplicationQuit()
    {
        ShutdownAudioEngine();
    }


}
