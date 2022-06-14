using System.Collections;
using System.Collections.Generic;
using UnityEngine;

using UnityEngine.EventSystems;

public class MouseLook : MonoBehaviour
{
    public float mouseSensitivity = 0.10f;

    public Transform playerBody;

    float xRotation = 0f;

    
    void Start()
    {
       // Cursor.lockState = CursorLockMode.Locked;
    }

  
    void Update()
    {
        float mouseX = 0;
        float mouseY = 0;

        
       


        if (Input.touchCount > 0  && !EventSystem.current.IsPointerOverGameObject(Input.GetTouch(0).fingerId))
        {
            if (Input.GetTouch(0).phase == TouchPhase.Moved)
            {
                mouseX = Input.GetTouch(0).deltaPosition.x;
                mouseY = Input.GetTouch(0).deltaPosition.y;
            }

           

        }

        //mouseX *= mouseSensitivity;
        //mouseY *= mouseSensitivity;
        //float mouseX = Input.GetAxis("Mouse X") * mouseSensitivity * Time.deltaTime;
        //float mouseY = Input.GetAxis("Mouse Y") * mouseSensitivity * Time.deltaTime;

        xRotation -= mouseY;
        xRotation = Mathf.Clamp(xRotation, -80f, 80f);

        transform.localRotation = Quaternion.Euler(xRotation, 0f, 0f);
        playerBody.Rotate(Vector3.up * mouseX);

       
    }
}
