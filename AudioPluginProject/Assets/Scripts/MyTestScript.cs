using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using UnityEditor;
using UnityEngine;


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
    public static extern void SetSource(Vector3 pos);

    [DllImport("MyAudioPlugin")]
    public static extern void ChangeVolumeByDistance(int channelID);

    [DllImport("MyAudioPlugin")]
    public static extern float ChangePanByOrientation(int channelID);

    [DllImport("MyAudioPlugin")]
    public static extern void SetMinMaxDistance(float min, float max);

    [DllImport("MyAudioPlugin")]
    public static extern float AngleValue();


    //public AudioClip firstClip;
    //public AudioClip secondClip;
    public GameObject listener;
    public GameObject source;

    int chID;
    

    void Start()
    {


        // string path = Application.streamingAssetsPath;
        //string firstPath = AssetDatabase.GetAssetPath(firstClip.GetInstanceID());
        // string secondPath = AssetDatabase.GetAssetPath(secondClip.GetInstanceID());

        string firstPath = Path.Combine(Application.streamingAssetsPath, "singing.wav");
        Debug.Log(" path " + firstPath);
        

        InitAudioEngine();
        int rez = LoadSound(firstPath, true);
       // Debug.Log("Rezult of LoadSound first: " + rez);

      

        //rez = LoadSound(secondPath, false);
        //Debug.Log("Rezult of LoadSound second: " + rez);

        chID = PlaySounds(firstPath);
        //Debug.Log("ChannelID of PlaySound first: " + chID);

        //rez = PlaySounds(secondPath, 0.0f);
        //Debug.Log("Rezult of PlaySound second: " + rez);
        //PlaySounds(secondPath, 0.0f);
      //  Debug.Log("Number of sounds: " + ReturnNumOfSounds());
        //Debug.Log("Number of channels: " + ReturnNumOfChannels());

        


        SetMinMaxDistance(5.0f, 50.0f);

       
        //Debug.Log("Angle value: " + AngleValue());




    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos_of_listener = listener.transform.position;
        Vector3 pos_of_source = source.transform.position;
        Vector3 nul = Vector3.zero;

        SetListener(pos_of_listener, listener.transform.forward, Vector3.up);
        SetSource(pos_of_source);
        ChangeVolumeByDistance(chID);
        float panVal = ChangePanByOrientation(chID);
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
