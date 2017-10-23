using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[RequireComponent(typeof(Collider))]
public class Draggable : MonoBehaviour
{
    public delegate void EventHandler();
    public event EventHandler CollideWithObject;

    void OnTriggerEnter(Collider collider)
    {
        CollideWithObject();
    }

    public void ClearHandlers()
    {
        CollideWithObject = null;
    }

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }
}
