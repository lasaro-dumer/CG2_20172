using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityStandardAssets.CrossPlatformInput;

public class ObjectInteraction : MonoBehaviour
{
    private GameObject grabbedObject;
    private float grabbedObjectSize;
    public float range = 2.5f;
    public float force = 5000f;
    public GameObject m_ElevatorController;
    [HideInInspector]
    public bool ElevatorMoving;
    private ElevatorController Elevator;
    public GameObject m_ElevatorDoorController;
    private ElevatorDoorController ElevatorDoor;
    private float throwForce;
    private bool grabbedShouldCollide;
    // Use this for initialization
    void Start()
    {
        grabbedObject = null;
        Elevator = m_ElevatorController.GetComponent<ElevatorController>();
        ElevatorDoor = m_ElevatorDoorController.GetComponent<ElevatorDoorController>();
    }

    public GameObject GetTargettedObject()
    {
        Ray camRay = new Ray(Camera.main.transform.position, Camera.main.transform.forward);
        RaycastHit hit;
        if (Physics.Raycast(camRay, out hit, range))
            return hit.collider.gameObject;
        return null;
    }

    private void TryGrabObject(GameObject grabObject)
    {
        if (grabObject == null || !CanGrab(grabObject))
            return;
        grabbedObject = grabObject;
        grabbedObjectSize = grabObject.GetComponent<Renderer>().bounds.size.magnitude;
        throwForce = force / grabbedObject.GetComponent<Rigidbody>().mass;
        Draggable drag = grabbedObject.GetComponent<Draggable>();
        if (drag == null)
        {
            grabbedObject.AddComponent<Draggable>();
            drag = grabbedObject.GetComponent<Draggable>();
        }
        drag.ClearHandlers();
        drag.CollideWithObject += () =>
        {
            DropObject();
        };
    }

    private void DropObject()
    {
        if (grabbedObject == null)
            return;
        if (grabbedObject.GetComponent<Rigidbody>() != null)
            grabbedObject.GetComponent<Rigidbody>().velocity = Vector3.zero;
        grabbedObject = null;
        grabbedShouldCollide = false;
    }

    private void ThrowObject()
    {
        grabbedObject.GetComponent<Rigidbody>().AddForce(Camera.main.transform.forward * throwForce);
        grabbedObject = null;
        grabbedShouldCollide = false;
    }

    public bool CanGrab(GameObject candidate)
    {
        bool can = candidate.GetComponent<Rigidbody>() != null;
        can = can && (candidate.layer == LayerMask.NameToLayer("InteractiveObjs"));
        can = can && (candidate.tag == "grabbable");
        return can;
    }

    // Update is called once per frame
    void Update()
    {
        ElevatorMoving = Elevator.ElevatorMoving;
        if (CrossPlatformInputManager.GetButtonDown("Grab"))
        {
            if (grabbedObject == null)
                TryGrabObject(GetTargettedObject());
            else
                DropObject();
        }
        //else if (CrossPlatformInputManager.GetButtonDown("Throw"))
        //{
        //    if (grabbedObject != null)
        //        ThrowObject();
        //}
        else if (CrossPlatformInputManager.GetButtonDown("Use"))
        {
            Usable usable = TryGetUsableObject(GetTargettedObject());
            if (usable != null)
            {
                Debug.Log("found usable");
                usable.Use();
            }
            else
            {
                Debug.Log("didn't find usable");
            }
        }
        //else if (CrossPlatformInputManager.GetButtonDown("GoUp"))
        //{
        //    Debug.Log("GoUp pressed");
        //    if (!ElevatorMoving)
        //    {
        //        //Call == "ElevatorUp" && 
        //        Elevator.ElevatorGO(ElevatorController.ElevatorDirection.Up);
        //    }
        //}
        //else if (CrossPlatformInputManager.GetButtonDown("GoDown"))
        //{
        //    Debug.Log("GoDown pressed");
        //    if (!ElevatorMoving)
        //    {
        //        //Call == "ElevatorDown" && 
        //        Elevator.ElevatorGO(ElevatorController.ElevatorDirection.Down);
        //    }
        //}
        //else if (CrossPlatformInputManager.GetButtonDown("ToogleElevatorDoor"))
        //{
        //    Debug.Log("ToogleElevatorDoor pressed");
        //    if (ElevatorDoor.IsOpen)
        //    {
        //        ElevatorDoor.CloseDoor();
        //    }
        //    else
        //    {
        //        ElevatorDoor.OpenDoor();
        //    }
        //}


        if (grabbedObject != null)
        {
            Vector3 newPosition = Camera.main.transform.position + Camera.main.transform.forward * range;
            grabbedObject.transform.position = newPosition;
        }
    }

    private Usable TryGetUsableObject(GameObject useObject)
    {
        if (useObject == null || !CanUse(useObject))
            return null;
        Usable usable = useObject.GetComponent<Usable>();
        return usable;
    }

    private bool CanUse(GameObject candidate)
    {
        bool can = candidate.GetComponent<Rigidbody>() != null;
        can = can && (candidate.layer == LayerMask.NameToLayer("InteractiveObjs"));
        can = can && (candidate.tag == "usable");
        return can;
    }

    private void OnTriggerEnter(Collider other)
    {
        Debug.Log("hello " + grabbedShouldCollide + " " + Time.realtimeSinceStartup);
    }
}
