using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElevatorDoorController : MonoBehaviour
{

    [Tooltip("Door move speed.")]
    public float doorSpeed = 0.05f;
    public bool startClosed = true;
    [Tooltip("Position when door is closed.")]
    public Transform m_ClosedPosition;
    private Transform ClosedPosition;
    [Tooltip("Position when door is open.")]
    public Transform m_OpenPosition;
    private Transform OpenPosition;
    private bool doorOpen;
    private string doorDirection;

    public bool DoorMoving { get; private set; }
    public bool IsOpen { get { return doorOpen; } }

    // Use this for initialization
    void Start()
    {
        ClosedPosition = m_ClosedPosition;
        OpenPosition = m_OpenPosition;
        doorOpen = !startClosed;
        if (startClosed)
            transform.position = ClosedPosition.position;
        else
            transform.position = OpenPosition.position;
    }
    private int ctrl = 0;
    public void FixedUpdate()
    {
        Transform newPosition = TargetPosition(doorDirection);
        if (doorDirection == "Open" || doorDirection == "Close")
        {
            DoorMoving = true;
            transform.position = Vector3.MoveTowards(transform.position, newPosition.position, doorSpeed);
        }

        if (this.transform.position == newPosition.position)
        {
            DoorMoving = false;
        }
        if (this.transform.position == ClosedPosition.position)
        {
            doorOpen = false;
        }
        if (this.transform.position == OpenPosition.position)
        {
            doorOpen = true;
        }

        //if (ElevatorMoving && !isMoved)
        //{
        //    if (!m_soundplayed)
        //    {
        //        this.GetComponent<AudioSource>().clip = ElevatorSounds.ElevatorStartMoving;
        //        this.GetComponent<AudioSource>().Play();
        //        m_soundplayed = true;
        //    }
        //    isMoved = true;
        //}
        //if (this.transform.position == ElevatorFloors[floorNumber].position && !ElevatorMoving && isMoved)
        //{
        //    if (!soundplayed)
        //    {
        //        this.GetComponent<AudioSource>().Stop();
        //        this.GetComponent<AudioSource>().PlayOneShot(ElevatorSounds.ElevatorStop);
        //        soundplayed = true;
        //    }
        //    isMoved = false;
        //}
    }

    private Transform TargetPosition(String target)
    {
        if (target == "Close")
            return ClosedPosition;
        else if (target == "Open")
            return OpenPosition;
        else
            return this.transform;
    }

    // Update is called once per frame
    void Update()
    {

    }

    internal void CloseDoor()
    {
        doorDirection = "Close";
    }

    internal void OpenDoor()
    {
        doorDirection = "Open";
    }
}
