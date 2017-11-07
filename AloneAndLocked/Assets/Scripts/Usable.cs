using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityStandardAssets.CrossPlatformInput;

public class Usable : MonoBehaviour
{

    public GameObject m_TargetController;

    // Use this for initialization
    void Start()
    {

    }


    public void Use()
    {
        ElevatorDoorController elevatorDoor = null;
        ElevatorButton elevatorButton = null;
        if (m_TargetController != null)
        {
            elevatorDoor = m_TargetController.GetComponent<ElevatorDoorController>();
            elevatorButton = m_TargetController.GetComponent<ElevatorButton>();
        }
        else
        {
            elevatorDoor = this.gameObject.GetComponent<ElevatorDoorController>();
            elevatorButton = this.gameObject.GetComponent<ElevatorButton>();
        }

        if (elevatorDoor != null)
        {
            Debug.Log("ToogleElevatorDoor pressed");
            if (elevatorDoor.IsOpen)
            {
                elevatorDoor.CloseDoor();
            }
            else
            {
                elevatorDoor.OpenDoor();
            }
        }
        if (elevatorButton != null)
        {
            Debug.Log("elevatorButton pressed");
            elevatorButton.Use();
        }
    }

    // Update is called once per frame
    void Update()
    {

    }
}
