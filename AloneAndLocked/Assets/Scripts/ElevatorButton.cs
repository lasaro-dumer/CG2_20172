using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ElevatorButton : MonoBehaviour {

    public GameObject m_ElevatorController;
    private ElevatorController Elevator;
    public ElevatorController.ElevatorDirection direction;

    // Use this for initialization
    void Start ()
    {
        Elevator = m_ElevatorController.GetComponent<ElevatorController>();

    }

    public void Use()
    {
        Elevator.ElevatorGO(direction);
    }

	// Update is called once per frame
	void Update () {
		
	}
}
