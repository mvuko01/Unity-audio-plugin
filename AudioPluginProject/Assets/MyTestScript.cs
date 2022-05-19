using System.Collections;
using System.Collections.Generic;
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
    public static extern int PlaySounds(string path, float volume);

    [DllImport("MyAudioPlugin")]
    public static extern int ReturnNumOfSounds();

    [DllImport("MyAudioPlugin")]
    public static extern int ReturnNumOfChannels();

    [DllImport("MyAudioPlugin")]
    public static extern void SetListener(Vector3 pos, Vector3 ori);

    [DllImport("MyAudioPlugin")]
    public static extern void SetSource(Vector3 pos);

    [DllImport("MyAudioPlugin")]
    public static extern void ChangeVolumeByDistance(int channelID);

    [DllImport("MyAudioPlugin")]
    public static extern void SetMinMaxDistance(float min, float max);

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerX();

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerY();

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerZ();

    public AudioClip firstClip;
    public AudioClip secondClip;
    public GameObject listener;
    public GameObject source;

    int chID;
    

    void Start()
    {

        string firstPath = AssetDatabase.GetAssetPath(firstClip.GetInstanceID());
        string secondPath = AssetDatabase.GetAssetPath(secondClip.GetInstanceID());

        

        InitAudioEngine();
        int rez = LoadSound(firstPath, true);
        Debug.Log("Rezult of LoadSound first: " + rez);

      

        //rez = LoadSound(secondPath, false);
        //Debug.Log("Rezult of LoadSound second: " + rez);

        chID = PlaySounds(firstPath, 0.0f);
        Debug.Log("ChannelID of PlaySound first: " + chID);

        //rez = PlaySounds(secondPath, 0.0f);
        //Debug.Log("Rezult of PlaySound second: " + rez);
        //PlaySounds(secondPath, 0.0f);
        Debug.Log("Number of sounds: " + ReturnNumOfSounds());
        Debug.Log("Number of channels: " + ReturnNumOfChannels());




        SetMinMaxDistance(5.0f, 30.0f);
        

        
        

    }

    // Update is called once per frame
    void Update()
    {
        Vector3 pos_of_listener = listener.transform.position;
        Vector3 pos_of_source = source.transform.position;
        Vector3 nul = Vector3.zero;

        SetListener(pos_of_listener, nul);
        SetSource(pos_of_source);
        ChangeVolumeByDistance(chID);
        UpdateAudioEngine();
        
    }

    private void OnApplicationQuit()
    {
        ShutdownAudioEngine();
    }


}
