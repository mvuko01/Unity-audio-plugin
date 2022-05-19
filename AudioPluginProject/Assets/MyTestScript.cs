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
    public static extern void SetListener(Vector3 pos);

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerX();

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerY();

    [DllImport("MyAudioPlugin")]
    public static extern float GetListenerZ();

    public AudioClip firstClip;
    public AudioClip secondClip;
    public GameObject listener;

    void Start()
    {

        string firstPath = AssetDatabase.GetAssetPath(firstClip.GetInstanceID());
        string secondPath = AssetDatabase.GetAssetPath(secondClip.GetInstanceID());

        

        InitAudioEngine();
        int rez = LoadSound(firstPath, true);
        Debug.Log("Rezult of LoadSound first: " + rez);

      

        rez = LoadSound(secondPath, false);
        Debug.Log("Rezult of LoadSound second: " + rez);

        rez = PlaySounds(firstPath, 0.0f);
        Debug.Log("Rezult of PlaySound first: " + rez);

        rez = PlaySounds(secondPath, 0.0f);
        Debug.Log("Rezult of PlaySound second: " + rez);
        //PlaySounds(secondPath, 0.0f);
        Debug.Log("Number of sounds: " + ReturnNumOfSounds());
        Debug.Log("Number of channels: " + ReturnNumOfChannels());

        Vector3 pos = listener.transform.position;
        Debug.Log("Listener is in position: "+ pos);
        SetListener(pos);
        Debug.Log("Listener x= " + GetListenerX());
        Debug.Log("Listener y= " + GetListenerY());
        Debug.Log("Listener z= " + GetListenerZ());



    }

    // Update is called once per frame
    void Update()
    {

       UpdateAudioEngine();
        
    }

    private void OnApplicationQuit()
    {
        ShutdownAudioEngine();
    }


}
